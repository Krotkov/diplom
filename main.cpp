#include <iostream>
#include <graphic.h>
#include <getopt.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
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
    for (int i = 3; i < 23; i++) {
        x.push_back((i + 1) * 1.0 / 4);
    }

    PolarCode code{n, k, erasure};
    GausChannel channel(n, k, noise);
    SC decoder(code);

//    auto kernel = createExtendedBchKernel(16);
    std::ifstream in("../kernels/32-1.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    PolarCode code1(n, k, kernel);
    SCViterbi decoder1(code1);

//    CrcPolarCode code1{n, k, 16};
//    SCFlipArikan decoder1{code1, 10};

    auto results = build_graphic(code, channel, decoder, max_word_num, x);
    auto results1 = build_graphic(code1, channel, decoder1, max_word_num, x);

    print_for_python(x);
    print_for_python(results);
    print_for_python(results1);
    return 0;
}
