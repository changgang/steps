# Simulation Toolkit for Electrical Power Systems
- Maintainer: Changgang Li <lichgang@sdu.edu.cn> from School of Electrical Engineering, Shandong University.
- Documentation: http://steps.drli.group:310
- Date of Readme: August 25, 2019

## Major functions
Simulation Toolkit for Electrical Power Systems (STEPS) is an open source power system simulator designed for balanced large-scale AC-DC hybrid power system analysis.  
Currently, it supports powerflow solution and dynamic simulation. In the future, short circuit analysis and eigenvalue analysis will be supported with no timetable.  
The data format of STEPS is improved based on PSS/E raw and dyr data format. BPA dat format is also supported.  
You are encourage to join us for further development.
## Prerequisite
1. Install Code::Blocks.
2. Install C and C++ compiler supprting C++17. GCC is recommended.
  - For Unix-like systems, follow guides to install gcc and g++ with the latest version.
  - For Windows system, MinGW-64 is suggested.
    - Go to http://mingw-w64.org/ and go to its sourceforge download site for the latest version.
    - Install 32-bit version or 64-bit version as you like. 64-bit version is recommended.
3. Start Code::Blocks, and configure its toolchain exectuable.
4. Install Python if you want to try the stepspy module. 64-bit version is recommended. However, if you want to use 32-bit dynamic library of STEPS, you should install Python of the 32-bit version.

## How to build
1. Select Linear Algebra solver to use
  1. Open STEPS.cbp in the code/ folder with Code::Blocks.
  2. Go to header/basic/sparse\_matrix\_define.h. Comment the solver you don't want to use. For example, if you want to keep only c\_sparse, comment umfpack and klu.
  3. Go to source/basic/sparse\_matrix\_klu.cpp and umfpack.cpp, disable 'compile' and 'link' options of the two files.
  4. Save STEPS.cbp.
2. Compile Linear Algebra solver.
  1. Open the Linear Algebra solver project. For example, if you choose to use CSparse as default Linear Algebra solver, open csparse.cbp in the code/ folder with Code::Blocks.
  2. Go 'Project'-'Properties'-'Build targets'and set type as 'static library'.
  3. Compile CSparse. You should get a libCSparse.a in folder build/bin/Release/.
3. Compile CppTest.
  1. Open cpptest.cbp in the code/ folder with Code::Blocks, set build target as 'static library'.
  2. Compile CppTest. You should get a libCppTest.a in folder build/bin/Release/.
4. Build STEPS
  1. Open STEPS.cbp in the code/ folder with Code::Blocks. 
  2. Go 'Project'-'Build options'-'Linker settings', and add the libCsparse.a and libCppTest.a library to 'Link libraries'.
  3. Go 'Project'-'Properties'-'Build Target', and select build target
    - If you want to compile STEPS as dynamic library, select 'Dynamic library', and set the output file name as 'libsteps.so' for Unix-like systems or 'libsteps.dll' for Windows systems.
    - If you want to compile STEPS as console application, select 'Console application'.
  4. If the target is dynamic library, disable 'Link' option for Source files with leading name of 'main'. If the target is console application, enable 'Link' option for only one of Source files with leading name of 'main'.
  5. Compile the STEPS. You should get the output file in folder build/bin/Release/.
  6. When compiling in Windows, remove -no-pie and -lgfortran options and remove libKLU.a, libBLAS.a, libUMFPACK.a

Here is a complete procedure of how to build STEPS.  
1. Compile BLAS as static library. You can find BLAS.cbp in the code/ folder.
2. Compile UMFPACK as static library. You can find UMFPACK.cbp in the code/ folder.
3. Compile CSparse as static library. You can find CSparse.cbp in the code/ folder.
4. Compile CppTest as static library. You can find CppTest.cbp in the code/ folder.
5. Compile STEPS as dynamic library or exectuable file. Include libUMFPACK.a, libBLAS.a, libCSparse.a, and libCpptest.a. Put libBLAS.a after libUMFPACK.a.

## How to use stepspy
stepspy is a Python module of advanced APIs of STEPS. It enables most of STEPS applications. It is recommended to use stepspy in Python 3 of 64-bit version, though Python 2 is also supported.

stepspy has been uploaded to pypi.org, and can be installed via:
- pip3 install stepspy 
- python -m pip install stepspy

Both parallel and serial library files should be built and copied to the stepspy folder.