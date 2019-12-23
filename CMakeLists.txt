cmake_minimum_required(VERSION 2.8.12)
project(num2re)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -Wall -std=c++11 -fPIC")
SET(CMAKE_CXX_FLAGS "-I $ENV{JAVA_HOME}/include -I $ENV{JAVA_HOME}/include/linux -I $ENV{JAVA_HOME}/include/darwin -D_REENTRANT")

add_library(num2re SHARED num2re.cpp num2re.h)
add_library(JNInum2re SHARED num2re.cpp num2re.h com_ztesoft_bss_zsmart_ucc_cc_common_utils_Number2RegularHelper.cpp com_ztesoft_bss_zsmart_ucc_cc_common_utils_Number2RegularHelper.h)

add_executable(num2re_test main.cpp num2re)
