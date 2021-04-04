#include "graphic.h"
#include <iostream>
#include <Code/ReedMuller/ReedMullerCode.h>
#include <Decoder/viterbi/Viterbi.h>
#include "utils/utils.h"

int main(int argc, char *argv[]) {
    int n = 32, k = 16, d = 8;
    int max_word_num = 100000;

    std::vector<double> ns;
    ns.reserve(25);
    for (int i = 0; i < 25; i++) {
        ns.push_back(i * 1. / 4);
    }

    ReedMullerCode code{n, k, d};
    GausChannel channel(n, k, 1);
    Viterbi decoder(code.getG());

    auto results = build_graphic(code, channel, decoder, max_word_num, ns);

    std::cout << "[";
    for (int i = 0; i < ns.size(); i++) {
        std::cout << ns[i];
        if (i + 1 != ns.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
    std::cout << "[";
    for (int i = 0; i < results.size(); i++) {
        std::cout << results[i];
        if (i + 1 != results.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]";
    return 0;
}
