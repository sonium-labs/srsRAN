srsRAN
======
srsRAN is a 4G/5G software radio suite developed by [SRS](http://www.srs.io).

This version has been modified by [Daniel Dole-Muinos](mailto:ddolemuinos@gmail.com) for use in signals of opportunity range-finding.

Minor changes have been made in the following areas:
- Disable options in CMakeLists.txt to avoid compiling with unneeded libraries
- Comment out MSG3 tx generation function (naive solution, but works)
- Adds printout of calculated range given TA value after RAR reception
- Enables Nuand BT-200 rx amp/bias tee on startup of srsUE (shown by green led)

Approaches for repeated RACH transmission are outlined in the "Ways to Trigger RACH" section in the [collected notes](https://docs.google.com/document/d/1BVgUOT2Mcy1JE5aCjtZHGem7sNVcIZMlFGTSadm2Tjc/edit#). Manually-triggered TA sending has not been included in this version as there are many different layers this can be triggered in.

Add'l Info 
======
See the [srsRAN project pages](https://www.srsran.com) for information, guides and project news.

The srsRAN suite includes:
  * srsUE - a full-stack SDR 4G/5G-NSA UE application (5G-SA coming soon)
  * srsENB - a full-stack SDR 4G/5G-NSA eNodeB application (5G-SA coming soon)
  * srsEPC - a light-weight 4G core network implementation with MME, HSS and S/P-GW

For application features, build instructions and user guides see the [srsRAN documentation](https://docs.srsran.com).

For license details, see LICENSE file.

Support
=======

Mailing list: https://lists.srsran.com/mailman/listinfo/srsran-users
