cmake_minimum_required(VERSION 2.9)
project(diplom)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(${CMAKE_SOURCE_DIR})

add_library(polar_code STATIC Message/Message.cpp
        Message/Message.h
        Channel/Gaus/GausChannel.cpp
        Channel/Gaus/GausChannel.h
        Matrix/Matrix.cpp
        Matrix/Matrix.h
        utils/utils.h
        Symbol/Symbol.cpp
        Symbol/Symbol.h
        utils/utils.cpp
        Decoder/SCs/SC/SC.cpp
        Decoder/SCs/SC/SC.h
        Decoder/viterbi/Viterbi.cpp
        Decoder/viterbi/Viterbi.h
        Channel/Channel.h
        Channel/PerfectGauss/PerfectGauss.cpp
        Channel/PerfectGauss/PerfectGauss.h
        Code/ReedMuller/ReedMullerCode.h
        Code/PolarCode/PolarCode.h
        Code/PolarCode/PolarCode.cpp
        Decoder/Decoder.h
        Decoder/Perebor/Perebor.cpp
        Decoder/Perebor/Perebor.h
        Polynom/Polynom.h
        BCH/BchKernel.h
        BCH/BchKernel.cpp
        Decoder/SCs/SCViterbi/SCViterbi.h
        Decoder/SCs/SCViterbi/SCViterbi.cpp
        Decoder/SCs/SCBase.h
        Decoder/SCs/SCBase.cpp
        Code/CRC/CrcPolarCode.h
        Code/CRC/CrcPolarCode.cpp
        Code/Code.h
        Polynom/Polynom.cpp Decoder/SCs/SCFlip/SCFlipArikan.cpp Decoder/SCs/SCFlip/SCFlipArikan.h)

add_executable(diplom main.cpp graphic.h Polynom/Polynom.cpp)

target_link_libraries(diplom polar_code)

add_executable(test test.cpp)

target_link_libraries(test polar_code)