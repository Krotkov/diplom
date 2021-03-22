cmake_minimum_required(VERSION 3.17)
project(diplom)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "-pthread")

#SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread")

#add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(diplom main.cpp Message/Message.cpp Message/Message.h Channel/Channel.cpp Channel/Channel.h Matrix/Matrix.cpp Matrix/Matrix.h utils/utils.h Code/PolarCode.cpp Code/PolarCode.h Symbol/Symbol.cpp Symbol/Symbol.h utils/utils.cpp)