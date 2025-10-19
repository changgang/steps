# script for cmake
# Author: Changgang Li
# Date: Oct. 12, 2025

# set cmake version required. 
# Since I am using Ubuntu 24.04.3 LTS with kernel 6.14.0, cmake of version 3.28 is supported.
# You can change it if you have cmake with lower version. But you need to test by yourself to check if lower version works.
cmake_minimum_required(VERSION 3.28)

# project
project(STEPS LANGUAGES C CXX)

# set build type: Debug or Release
# set(CMAKE_BUILD_TYPE Debug)
# set toolchain options
#
# This part SHOULD be modified for different platforms
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows") # use MinGW for Windows
    set(CMAKE_TOOLCHAIN_DIR "D:/Program Files/mingw-64/x86_64-15.1.0-posix-seh-ucrt-rt_v12-rev0/bin")  # MODIFY here if necessary
    set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_DIR}/gcc.exe)
    set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_DIR}/g++.exe)
    set(CMAKE_MAKE_PROGRAM ${CMAKE_TOOLCHAIN_DIR}/mingw32-make.exe)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux") # use gcc for Linux
    set(CMAKE_TOOLCHAIN_DIR "/usr/bin") # MODIFY here if necessary
    set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_DIR}/gcc)
    set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_DIR}/g++)
    set(CMAKE_MAKE_PROGRAM ${CMAKE_TOOLCHAIN_DIR}/make)
endif()

# set C and C++ standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 14)

# Set C compiler flags
set(CMAKE_C_FLAGS   "${CAMKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS   "${CAMKE_C_FLAGS} -pg")
# same flags for release
set(CMAKE_C_FLAGS_RELEASE   "${CAMKE_C_FLAGS}")
# add -pg to profile for debug
set(CMAKE_C_FLAGS_DEBUG   "${CAMKE_C_FLAGS}")

# Set C++ compiler flags
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp -static-libgcc -static-libstdc++ -march=native -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -malign-double -minline-all-stringops")
# same flags for release
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -pg")
# add -pg to profile for debug
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pg")

# from here, you are going to set up source directories of STEPS and its dependent third-party modules

# set SuiteSparse source directory
set(SUITESPARSE_SOURCE_DIR "./code/thirdparty/SuiteSparse")

# build CSparse, for real sparse matrix operation
set(CSPARSE_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/CSparse)
file(GLOB CSPARSE_SOURCES "${CSPARSE_SOURCE_DIR}/Source/*.c")
# CSparse is built as static library
add_library(CSparse STATIC ${CSPARSE_SOURCES})
# add headers for CSparse
target_include_directories(CSparse PRIVATE ${CSPARSE_SOURCE_DIR}/Include)

# build CXSparse, for complex sparse matrix operation
set(CXSPARSE_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/CXSparse)
file(GLOB CXSPARSE_SOURCES "${CXSPARSE_SOURCE_DIR}/Source/*.c")
# CXSparse is built as static library
add_library(CXSparse STATIC ${CXSPARSE_SOURCES})
# add headers for CXSparse
target_include_directories(CXSparse PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(CXSparse PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)

# build CppTest, for unit test
set(CPPTEST_SOURCE_DIR "./code/thirdparty/cpptest")
file(GLOB CPPTEST_SOURCES "${CPPTEST_SOURCE_DIR}/*.cpp")
# CppTest is built as static library
add_library(CppTest STATIC ${CPPTEST_SOURCES})
# add headers for CppTest
target_include_directories(CppTest PRIVATE ${CPPTEST_SOURCE_DIR})

# build STEPS
set(STEPS_SOURCE_DIR "./code/steps")
# get all source files
file(GLOB_RECURSE STEPS_SOURCES "${STEPS_SOURCE_DIR}/source/*.cpp")
# if you want to remove those unit test codes, uncomment the following 2 lines
file(GLOB_RECURSE STEPS_TEST_SOURCES "${STEPS_SOURCE_DIR}/source/*test.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${STEPS_TEST_SOURCES})
#file(GLOB_RECURSE STEPS_METER_TEST_SOURCES "${STEPS_SOURCE_DIR}/source/meter/*test.cpp")
#list(REMOVE_ITEM STEPS_SOURCES ${STEPS_METER_TEST_SOURCES})
#file(GLOB_RECURSE STEPS_LVRT_TEST_SOURCES "${STEPS_SOURCE_DIR}/source/model/common_model/vrt_model/*test.cpp")
#list(REMOVE_ITEM STEPS_SOURCES ${STEPS_LVRT_TEST_SOURCES})
#file(GLOB_RECURSE STEPS_GAST2A_TEST_SOURCES "${STEPS_SOURCE_DIR}/source/model/sg_models/turbine_governors/GAST2A_test.cpp")
#list(REMOVE_ITEM STEPS_SOURCES ${STEPS_GAST2A_TEST_SOURCES})
#
# here are some special files need to to be removed
file(GLOB_RECURSE VSC_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*/vsc.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${VSC_CPP_SOURCES})
file(GLOB_RECURSE VSCHVDC1_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*/VSCHVDC1.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${VSCHVDC1_CPP_SOURCES})
file(GLOB_RECURSE MATRIX_UMFPACK_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*matrix_umfpack.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${MATRIX_UMFPACK_CPP_SOURCES})
file(GLOB_RECURSE MATRIX_KLU_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*matrix_klu.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${MATRIX_KLU_CPP_SOURCES})
# now you have basic sources for building shared library 
# add target: shared library
add_library(STEPS SHARED ${STEPS_SOURCES})
# add headers
target_include_directories(STEPS PRIVATE ${STEPS_SOURCE_DIR})
target_include_directories(STEPS PRIVATE ${CSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)
target_include_directories(STEPS PRIVATE ${CPPTEST_SOURCE_DIR})
set(KLU_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/KLU)
target_include_directories(STEPS PRIVATE ${KLU_SOURCE_DIR}/Include)
set(UMFPACK_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/UMFPACK)
target_include_directories(STEPS PRIVATE ${UMFPACK_SOURCE_DIR}/Include)
set(AMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/AMD)
target_include_directories(STEPS PRIVATE ${AMD_SOURCE_DIR}/Include)
set(COLAMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/COLAMD)
target_include_directories(STEPS PRIVATE ${COLAMD_SOURCE_DIR}/Include)
set(BTF_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/BTF)
target_include_directories(STEPS PRIVATE ${BTF_SOURCE_DIR}/Include)
set(STEPSCODE_SOURCE_DIR "./code")
target_include_directories(STEPS PRIVATE ${STEPSCODE_SOURCE_DIR})
# static libraries CSparse, CXSparse, and CppTest must be included.
target_link_libraries(STEPS CSparse CXSparse CppTest)

# to build STEPS as executable, you should include source with main() function
# for example, the following code shows how to add main_gprof_pf.cpp for compilation
list(REMOVE_ITEM STEPS_SOURCES ${STEPS_MAIN_SOURCE})
file(GLOB_RECURSE STEPS_MAIN_PF_SOURCE "${STEPS_SOURCE_DIR}/main_gprof_pf.cpp")
list(APPEND STEPS_SOURCES ${STEPS_MAIN_PF_SOURCE})
# add target: executable
add_executable(STEPS_gprof_pf ${STEPS_SOURCES})
# add headers
target_include_directories(STEPS_gprof_pf PRIVATE ${STEPS_SOURCE_DIR})
target_include_directories(STEPS_gprof_pf PRIVATE ${CSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_pf PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_pf PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)
target_include_directories(STEPS_gprof_pf PRIVATE ${CPPTEST_SOURCE_DIR})
set(KLU_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/KLU)
target_include_directories(STEPS_gprof_pf PRIVATE ${KLU_SOURCE_DIR}/Include)
set(UMFPACK_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/UMFPACK)
target_include_directories(STEPS_gprof_pf PRIVATE ${UMFPACK_SOURCE_DIR}/Include)
set(AMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/AMD)
target_include_directories(STEPS_gprof_pf PRIVATE ${AMD_SOURCE_DIR}/Include)
set(COLAMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/COLAMD)
target_include_directories(STEPS_gprof_pf PRIVATE ${COLAMD_SOURCE_DIR}/Include)
set(BTF_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/BTF)
target_include_directories(STEPS_gprof_pf PRIVATE ${BTF_SOURCE_DIR}/Include)
set(STEPSCODE_SOURCE_DIR "./code")
target_include_directories(STEPS_gprof_pf PRIVATE ${STEPSCODE_SOURCE_DIR})
# static libraries CSparse, CXSparse, and CppTest must be included.
target_link_libraries(STEPS_gprof_pf CSparse CXSparse CppTest)



# to build STEPS as executable, you should include source with main() function
# for example, the following code shows how to add main_gprof_dy_sd133.cpp for compilation
file(GLOB_RECURSE STEPS_MAIN_DY_SD133_SOURCE "${STEPS_SOURCE_DIR}/main_gprof_dy_sd133.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${STEPS_MAIN_PF_SOURCE})
list(APPEND STEPS_SOURCES ${STEPS_MAIN_DY_SD133_SOURCE})
# add target: executable
add_executable(STEPS_gprof_dy_sd133 ${STEPS_SOURCES})
# add headers
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${STEPS_SOURCE_DIR})
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${CSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${CPPTEST_SOURCE_DIR})
set(KLU_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/KLU)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${KLU_SOURCE_DIR}/Include)
set(UMFPACK_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/UMFPACK)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${UMFPACK_SOURCE_DIR}/Include)
set(AMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/AMD)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${AMD_SOURCE_DIR}/Include)
set(COLAMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/COLAMD)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${COLAMD_SOURCE_DIR}/Include)
set(BTF_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/BTF)
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${BTF_SOURCE_DIR}/Include)
set(STEPSCODE_SOURCE_DIR "./code")
target_include_directories(STEPS_gprof_dy_sd133 PRIVATE ${STEPSCODE_SOURCE_DIR})
# static libraries CSparse, CXSparse, and CppTest must be included.
target_link_libraries(STEPS_gprof_dy_sd133 CSparse CXSparse CppTest)


# to build STEPS as executable, you should include source with main() function
# for example, the following code shows how to add main_gprof_dy_nccc10k.cpp for compilation
file(GLOB_RECURSE STEPS_MAIN_DY_NCCC10K_SOURCE "${STEPS_SOURCE_DIR}/main_gprof_dy_nccc10k.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${STEPS_MAIN_DY_SD133_SOURCE})
list(APPEND STEPS_SOURCES ${STEPS_MAIN_DY_NCCC10K_SOURCE})
# add target: executable
add_executable(STEPS_gprof_dy_nccc10k ${STEPS_SOURCES})
# add headers
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${STEPS_SOURCE_DIR})
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${CSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${CPPTEST_SOURCE_DIR})
set(KLU_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/KLU)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${KLU_SOURCE_DIR}/Include)
set(UMFPACK_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/UMFPACK)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${UMFPACK_SOURCE_DIR}/Include)
set(AMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/AMD)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${AMD_SOURCE_DIR}/Include)
set(COLAMD_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/COLAMD)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${COLAMD_SOURCE_DIR}/Include)
set(BTF_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/BTF)
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${BTF_SOURCE_DIR}/Include)
set(STEPSCODE_SOURCE_DIR "./code")
target_include_directories(STEPS_gprof_dy_nccc10k PRIVATE ${STEPSCODE_SOURCE_DIR})
# static libraries CSparse, CXSparse, and CppTest must be included.
target_link_libraries(STEPS_gprof_dy_nccc10k CSparse CXSparse CppTest)