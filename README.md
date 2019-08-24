Simulation Toolkit for Electrical Power Systems

Maintainer: Changgang Li <lichgang@sdu.edu.cn>

Please go to http://steps.drli.group:310 for documentation.

How to build:
1) Select which Linear Algebra solver is to be used

Open STEPS.cbp and go to header/basic/sparse_matrix_define.h. Comment the solver you don't want to use.

For example, if you want to keep only c_sparse, comment out umfpack and klu.

Then go to source/basic/sparse_matrix_klu.cpp and umfpack.cpp, disable 'compile' and 'link' options of the two files.

2) Go compile the Linear Algebra solver.

For example, open csparse.cbp, and compile CSparse. The target should be 'static library'

3) Go compile the CppTest.

Open cpptest.cbp, and compile CppTest. The target should be 'static library'

4) Build STEPS

In the linker settings, you should include the above two static libraries libCSparse.a and libCpptest.a.

Then compile the STEPS.

5) If you want to build a 64bit version, use 64 bit MinGW and select the X64 version compiler.

If you want to compile a complete version, follow the steps:

1) Compile BLAS as static library

2) Compile UMFPACK as static library

3) Compile CSparse as static library

4) Compile CppTest as static library

5) Compile STEPS as dynamic library or exectuable file. Include libUMFPACK.a, libBLAS.a, libCSparse.a, and libCpptest.a. Put libBLAS.a after libUMFPACK.a.

When compiling in Windows, remove -no-pie and -lgfortran options and remove libKLU.a, libBLAS.a, libUMFPACK.a


