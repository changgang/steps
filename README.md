# Simulation Toolkit for Electrical Power Systems
- Maintainer: Changgang Li <lichgang@sdu.edu.cn> from School of Electrical Engineering, Shandong University.
- Trust the following repositories or other credible duplications:
  * STEPS: https://gitee.com/lichgang/steps/ and https://github.com/changgang/steps
  * stepspy: https://pypi.org/project/stepspy/
  * Documentation: https://github.com/changgang/steps/wiki/
- Reminder of major repository:
  * The github repository may lag behind the gitee repository due to some GIT PUSH issues. So, use gitee repository first.
  * The 'dev' branch is constantly updating while the 'main' branch is updated when major functions are added. Use 'main' branch for stable applications, and try 'dev' branch for new features.
- Date of Readme: March 24, 2024

# Reference
Please cite STEPS with the following papers:  
```[1] Changgang Li, Yue Wu, Hengxu Zhang, Hua Ye, Yutian Liu and Yilu Liu. "STEPS: A Portable Numerical Simulation Toolkit for Electrical Power System Dynamic Studies," in IEEE Transactions on Power Systems, vol. 36, no. 4, pp. 3216-3226, July 2021, doi: 10.1109/TPWRS.2020.3045102.```  
```[2] Changgang Li, Yun Hang, Yutian Liu, Vladimir Terzija. "Uneven Internal Time-Step Adjustment for Fast Power System Dynamic Simulations based on Trapezoidal Integration of Elementary Transfer Function Blocks," in International Journal of Electrical Power & Energy Systems, vol. 155, Part A, 109509, January 2024, doi: 10.1016/j.ijepes.2023.109509```
See: https://ieeexplore.ieee.org/document/9295409, https://www.sciencedirect.com/science/article/pii/S0142061523005665

## Major functions
Simulation Toolkit for Electrical Power Systems (STEPS) is an open source power system simulator designed for balanced large-scale AC-DC hybrid power system analysis.  
Currently, it supports powerflow solution, short circuit analysis, and dynamic simulation. In the future, eigenvalue analysis will be supported with no timetable.  
The data format of STEPS is improved based on PSS/E raw, seq, and dyr data format. 
Though BPA dat format is supported, it will be supported in the future version in stepspy and BPA data importer will be removed.[Sep. 13, 2020]
You are encouraged to join us for further development.

## Major versions
- v2.2.0. Add function to change dynamic simulation time step manually. stepspy is also updated however not pushed to pypi due to token issue. please use python/stepspy-current/stepspy.
- v2.1.0. Add function to check if NAN is detected in powerflow solver. stepspy is also updated. Compile option '-ffast-math' is disabled. 
- v2.0.0. Major version. (1) Short circuit functions added. Short circuit analysis with direct solution is supported. APIs are added to stepspy. Short circuit analysis with iterative solution is implemented in C++, and APIs will be added in later version.  (2) Two toolkit functions added: Correct Three Winding Transformer Impedance Logic and Blockwise Automatic Large Step Logic. If the 'Correct' logic is enabled, zero impedance winding of 3-winding transformer can be automatically fixed to avoid NaN error. If the 'Blockwise' logic is enabled, dynamic simulation time step can be increased for fast simulation (See Reference Paper [2]). The 'Blockwise' logic is implemented with changes of dynamic blocks of first order, differential, and lead-lag. (3) Option 'libsteps_file' is added to stepspy when creating STEPS() object when dynamic library with name different from 'libSTEPS' is used. (4) Some wind turbine model bugs are fixed. (5) stepspy module is also upgraded to v2.0.0.
- v1.6.0. Add VSC HVDC for powerflow and dynamic simulation. stepspy also updated. Released on April 13, 2022.
- v1.5.4. Update to improve performance. Released on Oct. 20, 2021.
- v1.5.2. Add new APIs to get generator related model internal variables. Start to add linearization codes. Released on March. 24, 2021.
- v1.5.1. Add new option export_full to functions of save_network_Y_matrix and APIs. stepspy is also updated. Start to add sequential parameters to STEPS. Released on Feb. 2, 2021.
- v1.5.0. Add new function to get basic and user meter value in STEPS kernel and stepspy. Add additional parameter vbase to bus_name2number in case multi buses have the same bus name. Released on Dec. 22, 2020.
- v1.4.1. Fix clear toolkit when reloading new powerflow data. Enable build initial zero Y matrix. Fix access violation in jacobian builder when updating seperate jacobian matrix. Add check of LU factorization failure. Special log file named 'blackhole' is supported to disable all log. Released on Dec. 7, 2020.
- v1.4. add function to add default GENCLS model for generators without sync generator model when checking missing models. Fix bug in IEEEG1 in case T1 or T2 is not 0. Released on Sep. 27, 2020.
- v1.3.1: Fix minor bug in IEEEG1 when T1 or T2 is non-zero. Released on Sep. 13, 2020. 
- v1.3: Fix minor bug in building Z matrix. Released on April 22, 2020. This version is available for better compatibility.
- v1.1: Improved performance version on March 8, 2020. About 20~30% improvement comparing to v1.0.
- v1.0: Improved performance version on Feb. 12, 2020. Support synchronous generator, wind turbine generator, (basic) PV unit, HVDC, static and dynamic loads.
- v0.0: Initialize project on March 21, 2018

## Dependency
STEPS depends on CSparse, CXSparse, CppTest, and RapidJSON. The CXSparse is modified to avoid confliction between CSparse and CXSparse by changing prefix cs_ and CS_ to cxs_ and CSX_ .
Though BLAS, CBLAS, and other SuiteSparse codes are included in the package, they will be removed in future.

## How to build
1. Build STEPS kernel: https://github.com/changgang/steps/wiki/How-to-build-STEPS-kernel
2. Install stepspy module to Python: https://github.com/changgang/steps/wiki/How-to-install-stepspy-in-Python