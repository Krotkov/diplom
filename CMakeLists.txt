cmake_minimum_required(VERSION 3.17)
project(diplom)

set(CMAKE_CXX_STANDARD 20)

add_executable(diplom main.cpp Message/Message.cpp Message/Message.h Symbol/Symbol.cpp Symbol/Symbol.h Channel/Channel.cpp Channel/Channel.h Construct/construct.h Construct/construct.cpp)