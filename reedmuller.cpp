#include <iostream>
#include <future>
#include <Code/ReedMuller/ReedMullerCode.h>
#include <Decoder/viterbi/Viterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include "Channel/Gaus/GausChannel.h"
#include "utils/utils.h"

double standart_main(int n, int k, int d, double noise, double erasure, int max_word_num, double max_word_error) {
    int error_count = 0;
    ReedMullerCode code{n, k, d};
//    GausChannel channel(n, k, noise);
    PerfectGauss channel;
    Message a, b;
    Viterbi decoder(code.getG());
    for (int i = 0; i < max_word_num; i++) {
        a = generateWord(k);
        a = code.encode(a);
        b = channel.runMessage(a);
        b = decoder.decode(b, channel);
        if (compare(a, b) > 0) {
            error_count++;
        }
    }
    return (error_count * 1.0) / max_word_num;
}

int main(int argc, char *argv[]) {
    int n = 32, k = 16, d = 8;
    double noise = 1, erasure = 0.5;
    int max_word_num = 100;
    double max_word_error = 1;

    std::vector<double> results;
    std::vector<double> x;
    results.reserve(20);
    for (int i = 3; i < 23; i++) {
        x.push_back((i + 1) * 1.0 / 4);
        results.push_back(standart_main(n, k, d, (i + 1) * 1.0 / 4, erasure, max_word_num, max_word_error));
    }
    std::cout << "[";
    for (int i = 0; i < 20; i++) {
        std::cout << x[i];
        if (i != 19) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
    std::cout << "[";
    for (int i = 0; i < 20; i++) {
        std::cout << results[i];
        if (i != 19) {
            std::cout << ",";
        }
    }
    std::cout << "]";

//    graph_values(n, k, noise, erasure, max_word_num, max_word_error);
//    standart_main(n, k, noise, erasure, max_word_num, max_word_error);
    return 0;
}
