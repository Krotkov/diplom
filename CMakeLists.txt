cmake_minimum_required(VERSION 3.17)
project(diplom)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "-pthread")
include_directories(${CMAKE_SOURCE_DIR})

add_executable(diplom main.cpp Message/Message.cpp Message/Message.h Channel/Channel.cpp Channel/Channel.h Matrix/Matrix.cpp Matrix/Matrix.h utils/utils.h Code/PolarCode.cpp Code/PolarCode.h Symbol/Symbol.cpp Symbol/Symbol.h utils/utils.cpp Decoder/SC/SC.cpp Decoder/SC/SC.h Decoder/viterbi/Viterbi.cpp Decoder/viterbi/Viterbi.h)

add_executable(viterbi viterbi.cpp Message/Message.cpp Message/Message.h Channel/Channel.cpp Channel/Channel.h Matrix/Matrix.cpp Matrix/Matrix.h utils/utils.h Code/PolarCode.cpp Code/PolarCode.h Symbol/Symbol.cpp Symbol/Symbol.h utils/utils.cpp Decoder/SC/SC.cpp Decoder/SC/SC.h Decoder/viterbi/Viterbi.cpp Decoder/viterbi/Viterbi.h)