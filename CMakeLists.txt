cmake_minimum_required(VERSION 4.1)

# project
project(STEPS LANGUAGES C CXX)
# set toolchain options
#
# This part SHOULD be modified for different platforms
#
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows") # use MinGW for Windows
    set(CMAKE_TOOLCHAIN_DIR "D:/Program Files/mingw-64/x86_64-15.1.0-posix-seh-ucrt-rt_v12-rev0/bin")  # MODIFY here if necessary
    set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_DIR}/gcc.exe)
    set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_DIR}/g++.exe)
    set(CMAKE_MAKE_PROGRAM ${CMAKE_TOOLCHAIN_DIR}/mingw32-make.exe)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux") # use gcc for Linux
    message("Linux detected")
    set(CMAKE_TOOLCHAIN_DIR "/usr/bin") # MODIFY here if necessary
    set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_DIR}/gcc)
    set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_DIR}/g++)
    set(CMAKE_MAKE_PROGRAM ${CMAKE_TOOLCHAIN_DIR}/make)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 14)

# Set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp -static-libgcc -static-libstdc++ -Wall -march=native -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -malign-double -minline-all-stringops")
# add -g for debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
# add -Os for release
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Os")


# set SuiteSparse source directory
set(SUITESPARSE_SOURCE_DIR "./code/thirdparty/SuiteSparse")

# build CSparse
set(CSPARSE_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/CSparse)
file(GLOB CSPARSE_SOURCES "${CSPARSE_SOURCE_DIR}/Source/*.c")
add_library(CSparse STATIC ${CSPARSE_SOURCES})
target_include_directories(CSparse PRIVATE ${CSPARSE_SOURCE_DIR}/Include)

# build CXSparse
set(CXSPARSE_SOURCE_DIR ${SUITESPARSE_SOURCE_DIR}/CXSparse)
file(GLOB CXSPARSE_SOURCES "${CXSPARSE_SOURCE_DIR}/Source/*.c")
add_library(CXSparse STATIC ${CXSPARSE_SOURCES})
target_include_directories(CXSparse PRIVATE ${CXSPARSE_SOURCE_DIR}/Include)
target_include_directories(CXSparse PRIVATE ${SUITESPARSE_SOURCE_DIR}/SuiteSparse_config)

# build CppTest
set(CPPTEST_SOURCE_DIR "./code/thirdparty/cpptest")
file(GLOB CPPTEST_SOURCES "${CPPTEST_SOURCE_DIR}/*.cpp")
add_library(CppTest STATIC ${CPPTEST_SOURCES})
target_include_directories(CppTest PRIVATE ${CPPTEST_SOURCE_DIR})

# build STEPS
set(STEPS_SOURCE_DIR "./code/steps")
file(GLOB_RECURSE STEPS_SOURCES "${STEPS_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE STEPS_TEST_SOURCES "${STEPS_SOURCE_DIR}/source/*test.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${STEPS_TEST_SOURCES})
file(GLOB_RECURSE VSC_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*/vsc.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${VSC_CPP_SOURCES})
file(GLOB_RECURSE VSCHVDC1_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*/VSCHVDC1.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${VSCHVDC1_CPP_SOURCES})
file(GLOB_RECURSE MATRIX_UMFPACK_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*matrix_umfpack.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${MATRIX_UMFPACK_CPP_SOURCES})
file(GLOB_RECURSE MATRIX_KLU_CPP_SOURCES "${STEPS_SOURCE_DIR}/source/*matrix_klu.cpp")
list(REMOVE_ITEM STEPS_SOURCES ${MATRIX_KLU_CPP_SOURCES})
add_library(STEPS SHARED ${STEPS_SOURCES})
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
target_link_libraries(STEPS CSparse CXSparse CppTest)



