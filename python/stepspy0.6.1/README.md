stepspy: a Python module of Simulation Toolkit for Electrical Power Systems

Author: Changgang Li <lichgang@sdu.edu.cn> from the School of Electrical Engineering, Shandong University, China

stepspy provides almost all functions of STEPS (https://github.com/changgang/steps). It calls dynamic library of STEPS, and wraps it to advanced functions.

stepspy can now be automatically installed via pip with:

python -m pip install stepspy

After install the stepspy, you still need to install the dynamic library of STEPS.

If you want to manually install stepspy, follow the instructions:
1. Go to https://github.com/changgang/steps to download or fork the latest version of STEPS.
2. Compile STEPS into dynamic library following instructions of STEPS.
3. Go to python/ folder of STEPS, and copy the latest version of stepspy/ to PYTHONPATH/Lib/site-packages/.
4, Move the dynamic library of STEPS to stepspy/libsteps/ in the PYTHONPATH/Lib/site-packages/.
5, If the VC runtime is missing, download and install Microsoft Visual C++ 2017 Redistributable of 32 or 64 bit version.
5, If VC runtime or Mingw Runtime is missing, install vcredit or copy libwinpthread-1.dll from mingw compiler/bin/ to c:/windows/system32 and c:/windows/SysWOW64
