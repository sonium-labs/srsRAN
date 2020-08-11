/*
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * This file is part of srsLTE.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef SRSLTE_ADT_UTILS_H
#define SRSLTE_ADT_UTILS_H

namespace srslte {

#if defined(__cpp_exceptions) && (1 == __cpp_exceptions)
class bad_type_access : public std::runtime_error
{
public:
  explicit bad_type_access(const std::string& what_arg) : runtime_error(what_arg) {}
  explicit bad_type_access(const char* what_arg) : runtime_error(what_arg) {}
};

#define THROW_BAD_ACCESS(msg) throw bad_type_access{msg};
#else
#define THROW_BAD_ACCESS(msg)                                                                                          \
  fprintf(stderr, "ERROR: exception thrown with %s", msg);                                                             \
  std::abort()
#endif

} // namespace srslte

#endif // SRSLTE_ADT_UTILS_H
