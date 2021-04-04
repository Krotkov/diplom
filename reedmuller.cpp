#include "graphic.h"
#include <iostream>
#include <Code/ReedMuller/ReedMullerCode.h>
#include <Decoder/viterbi/Viterbi.h>
#include "utils/utils.h"

int main(int argc, char *argv[]) {
    int n = 32, k = 16, d = 8;
    int max_word_num = 10000;

    std::vector<double> ns;
    ns.reserve(25);
    for (int i = 0; i < 25; i++) {
        ns.push_back(i * 1. / 4);
    }

    ReedMullerCode code{n, k, d};
    GausChannel channel(n, k, 0);
    Viterbi decoder(code.getG());

    auto results = build_graphic(code, channel, decoder, max_word_num, ns);

    print_for_python(ns);
    print_for_python(results);
    return 0;
}
