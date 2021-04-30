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
        Code/PolarCode/PolarCodeWithArikan.cpp
        Code/PolarCode/PolarCodeWithArikan.h
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
        Code/PolarCode.h Decoder/Decoder.h
        Decoder/Perebor/Perebor.cpp
        Decoder/Perebor/Perebor.h
        Polynom/Polynom.h
        BCH/BchKernel.h
        BCH/BchKernel.cpp
        Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h
        Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.cpp
        Decoder/SCs/SCViterbi/SCViterbi.h
        Decoder/SCs/SCViterbi/SCViterbi.cpp
        Decoder/SCs/SCBase.h
        Decoder/SCs/SCBase.cpp
        )

add_executable(diplom main.cpp graphic.h)

target_link_libraries(diplom polar_code)

add_executable(viterbi graphic.h)

target_link_libraries(viterbi polar_code)

add_executable(reedmuller reedmuller.cpp graphic.h)

target_link_libraries(reedmuller polar_code)

add_executable(bch bch.cpp)

target_link_libraries(bch polar_code)