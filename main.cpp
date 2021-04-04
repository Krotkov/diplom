#include <iostream>
#include <graphic.h>
#include <getopt.h>
#include "Channel/Gaus/GausChannel.h"
#include "Code/PolarCode/PolarCode.h"
#include "utils/utils.h"
#include "Decoder/SC/SC.h"

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

    auto results = build_graphic(code, channel, decoder, max_word_num, x);

    print_for_python(x);
    print_for_python(results);
    return 0;
}
