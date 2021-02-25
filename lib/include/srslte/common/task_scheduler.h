/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSLTE_TASK_SCHEDULER_H
#define SRSLTE_TASK_SCHEDULER_H

#include "interfaces_common.h"
#include "multiqueue.h"
#include "thread_pool.h"
#include "timers.h"

namespace srslte {

class task_scheduler
{
public:
  explicit task_scheduler(uint32_t default_extern_tasks_size = 512, uint32_t nof_timers_prealloc = 100) :
    external_tasks{default_extern_tasks_size}, timers{nof_timers_prealloc}
  {
    background_queue_id = external_tasks.add_queue();
  }
  task_scheduler(const task_scheduler&) = delete;
  task_scheduler(task_scheduler&&)      = delete;
  task_scheduler& operator=(const task_scheduler&) = delete;
  task_scheduler& operator=(task_scheduler&&) = delete;

  void stop() { external_tasks.reset(); }

  srslte::unique_timer get_unique_timer() { return timers.get_unique_timer(); }

  //! Creates new queue for tasks coming from external thread
  srslte::task_queue_handle make_task_queue() { return external_tasks.get_queue_handler(); }
  srslte::task_queue_handle make_task_queue(uint32_t qsize) { return external_tasks.get_queue_handler(qsize); }

  //! Delays a task processing by duration_ms
  void defer_callback(uint32_t duration_ms, std::function<void()> func) { timers.defer_callback(duration_ms, func); }

  //! Enqueues internal task to be run in next tic
  void defer_task(srslte::move_task_t func) { internal_tasks.push_back(std::move(func)); }

  //! Defer the handling of the result of a background task to next tic
  void notify_background_task_result(srslte::move_task_t task)
  {
    // run the notification in next tic
    external_tasks.push(background_queue_id, std::move(task));
  }

  //! Updates timers, and run any pending internal tasks.
  //  CAUTION: Should be called in main thread
  void tic() { timers.step_all(); }

  //! Processes the next task in the multiqueue.
  //  CAUTION: This is a blocking call
  bool run_next_task()
  {
    srslte::move_task_t task{};
    if (external_tasks.wait_pop(&task) >= 0) {
      task();
      run_all_internal_tasks();
      return true;
    }
    run_all_internal_tasks();
    return false;
  }

  //! Processes the next task in the multiqueue if it exists.
  void run_pending_tasks()
  {
    run_all_internal_tasks();
    srslte::move_task_t task{};
    while (external_tasks.try_pop(&task) >= 0) {
      task();
      run_all_internal_tasks();
    }
  }

  srslte::timer_handler* get_timer_handler() { return &timers; }

private:
  void run_all_internal_tasks()
  {
    // Perform pending stack deferred tasks
    // Note: Using a deque because tasks can enqueue new tasks, which would lead to
    // reference invalidation in case of vector
    while (not internal_tasks.empty()) {
      internal_tasks.front()();
      internal_tasks.pop_front();
    }
  }

  int                     background_queue_id = -1; ///< Queue for handling the outcomes of tasks run in the background
  srslte::task_multiqueue external_tasks;
  srslte::timer_handler   timers;
  std::deque<srslte::move_task_t> internal_tasks; ///< enqueues stack tasks from within main thread. Avoids locking
};

//! Task scheduler handle given to classes/functions running within the main control thread
class task_sched_handle
{
public:
  task_sched_handle(task_scheduler* sched_) : sched(sched_) {}

  srslte::unique_timer get_unique_timer() { return sched->get_unique_timer(); }
  void                 notify_background_task_result(srslte::move_task_t task)
  {
    sched->notify_background_task_result(std::move(task));
  }
  void defer_callback(uint32_t duration_ms, std::function<void()> func)
  {
    sched->defer_callback(duration_ms, std::move(func));
  }
  void defer_task(srslte::move_task_t func) { sched->defer_task(std::move(func)); }

private:
  task_scheduler* sched;
};

//! Task scheduler handle given to classes/functions running outside of main control thread
class ext_task_sched_handle
{
public:
  ext_task_sched_handle(task_scheduler* sched_) : sched(sched_) {}

  srslte::unique_timer get_unique_timer() { return sched->get_unique_timer(); }
  void                 notify_background_task_result(srslte::move_task_t task)
  {
    sched->notify_background_task_result(std::move(task));
  }
  srslte::task_queue_handle make_task_queue() { return sched->make_task_queue(); }
  void                      defer_callback(uint32_t duration_ms, std::function<void()> func)
  {
    sched->defer_callback(duration_ms, std::move(func));
  }

private:
  task_scheduler* sched;
};

} // namespace srslte

#endif // SRSLTE_TASK_SCHEDULER_H
