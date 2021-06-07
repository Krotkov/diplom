#include <iostream>
#include <graphic.h>
#include <getopt.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <Code/CRC/CrcPolarCode.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>
#include "Channel/Gaus/GausChannel.h"
#include "utils/utils.h"
#include "Decoder/SCs/SC/SC.h"

int main(int argc, char *argv[]) {
    int n = 8, k = 4;
    double noise = 1, erasure = 0.5;
    int max_word_num = 100;
    double max_word_error = 1;

    int res = 0;
    while ((res = getopt(argc, argv, "n:k:g:e:w:m:")) != -1) {
        switch (res) {
            case 'n':
                n = std::stoi(optarg);
                continue;
            case 'k':
                k = std::stoi(optarg);
                continue;
            case 'g':
                noise = std::stod(optarg);
                continue;
            case 'e':
                erasure = std::stod(optarg);
                continue;
            case 'm':
                max_word_num = std::stoi(optarg);
                continue;
            case 'w':
                max_word_error = std::stod(optarg);
                continue;
            default:
                std::cerr << "unknown param\n";
                continue;
        }
    }

    std::vector<double> x;
    for (int i = 5; i <= 20; i++) {
        x.push_back((i + 1) * 1.0 / 5);
    }

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    std::ifstream in2("../kernels/16-1-r.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    auto rKernel = Matrix(in2);

    GausChannel channel(n, k, noise);

    PolarCode code{n, k, 0.5};
    SC decoder(code);

    CrcPolarCode code0{n, k, 10};
    SCFlip decoder0(code0, 0.5, 10, true);

//    CrcPolarCode code1 = code0;
//    SCFlip decoder1(code1, 0.5, 50, true);

    PolarCode code2(n, k, kernel, rKernel);
    SCViterbi decoder2(code2);

    CrcPolarCode code3{n, k, kernel, rKernel, 10};
    SCFlip decoder3(code3, 0.5, 10, true);

//    CrcPolarCode code4 = code3;
//    SCFlip decoder4(code4, 0.5, 50, true);

    auto results = build_graphic(code, channel, decoder, max_word_num, x);
    auto result0 = build_graphic(code0, channel, decoder0, max_word_num, x);
//    auto results1 = build_graphic(code1, channel, decoder1, max_word_num, x);
    auto results2 = build_graphic(code2, channel, decoder2, max_word_num, x);
    auto results3 = build_graphic(code3, channel, decoder3, max_word_num, x);
//    auto results4 = build_graphic(code4, channel, decoder4, max_word_num, x);

    print_for_python(x);
    print_for_python(results);
    print_for_python(result0);
//    print_for_python(results1);
    print_for_python(results2);
    print_for_python(results3);
//    print_for_python(results4);
    return 0;
}
