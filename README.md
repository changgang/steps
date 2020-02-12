# Simulation Toolkit for Electrical Power Systems
- Maintainer: Changgang Li <lichgang@sdu.edu.cn> from School of Electrical Engineering, Shandong University.
- Documentation: [N/A]
- Date of Readme: Jan. 20, 2020

## Major functions
Simulation Toolkit for Electrical Power Systems (STEPS) is an open source power system simulator designed for balanced large-scale AC-DC hybrid power system analysis.  
Currently, it supports powerflow solution and dynamic simulation. In the future, short circuit analysis and eigenvalue analysis will be supported with no timetable.  
The data format of STEPS is improved based on PSS/E raw and dyr data format. BPA dat format is also supported.  
You are encourage to join us for further development.

## Major versions
1. V0.0: Initialize project on March 21, 2018
2. V1.0: Improved performance version on Feb. 12, 2020. Support synchronous generator, wind turbine generator, (basic) PV unit, HVDC, static and dynamic loads.

## Dependency
STEPS depends on CSparse, CXSparse, CppTest, and RapidJSON. The CXSparse is modified to avoid confliction between CSparse and CXSparse by changing prefix cs_ and CS_ to cxs_ and CSX_ .
Though BLAS, CBLAS, and other SuiteSparse codes are included in the package, they will be removed in future.

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
1. Compile CSparse
  1. Open csparse.cbp in the code/ folder with Code::Blocks.
  2. Go 'Project'-'Properties'-'Build targets'and set type as 'static library'.
  3. Compile CSparse. You should get a libCSparse.a in folder build/bin/Release/.
2. Complie CXSparse
  1. Open cxsparse.cbp in the code/ folder with Code::Blocks
  2. Go 'Project'-'Properties'-'Build targets'and set type as 'static library'.
  3. Compile CXSparse. You should get a libCXSparse.a in folder build/bin/Release/.
3. Compile CppTest.
  1. Open cpptest.cbp in the code/ folder with Code::Blocks, set build target as 'static library'.
  2. Compile CppTest. You should get a libCppTest.a in folder build/bin/Release/.
4. Build STEPS
  1. Open STEPS.cbp in the code/ folder with Code::Blocks. 
  2. Go 'Project'-'Build options'-'Linker settings', and add the libCsparse.a, libCXSparse.a, and libCppTest.a library to 'Link libraries'.
  3. Go 'Project'-'Properties'-'Build Target', and select build target
    - If you want to compile STEPS as dynamic library, select 'Dynamic library', and set the output file name as 'libsteps.so' for Unix-like systems or 'libsteps.dll' for Windows systems.
    - If you want to compile STEPS as console application, select 'Console application'.
  4. If the target is dynamic library, disable 'Link' option for Source files with leading name of 'main'. If the target is console application, enable 'Link' option for only one of Source files with leading name of 'main'.
  5. Compile the STEPS. You should get the output file in folder build/bin/Release/.
  6. When compiling in Windows, remove -no-pie and -lgfortran options and remove libKLU.a, libBLAS.a, libUMFPACK.a

Here is a complete procedure of how to build STEPS.  
1. Compile CSparse as static library. You can find CSparse.cbp in the code/ folder.
2. Compile CXSparse as static library. You can find CXSparse.cbp in the code/ folder.
3. Compile CppTest as static library. You can find CppTest.cbp in the code/ folder.
4. Compile STEPS as dynamic library or exectuable file. Include libCSparse.a, libCXSparse.a, and libCpptest.a.

## How to use stepspy
stepspy is a Python module of advanced APIs of STEPS. It enables most of STEPS applications. It is recommended to use stepspy in Python 3 of 64-bit version.

stepspy has been uploaded to pypi.org, and can be installed via:
- pip3 install stepspy 
- python -m pip install stepspy

Remember to copy the built libSTEPS.dll or libSTEPS.so in release folder to the stepspy folder.