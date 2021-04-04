//#include "graphic.h"
#include <iostream>
#include <future>
#include <Code/ReedMuller/ReedMullerCode.h>
#include <Decoder/viterbi/Viterbi.h>
#include "Channel/Gaus/GausChannel.h"
#include "utils/utils.h"

double standart_main(int n, int k, int d, double noise, int max_word_num) {
    int error_count = 0;
    ReedMullerCode code{n, k, d};
    GausChannel channel(n, k, noise);
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
    int max_word_num = 10000000;

    std::vector<std::future<double>> results;
    std::vector<double> x;
    for (int i = 0; i < 31; i++) {
        x.push_back(i * 1. / 4);
        results.push_back(std::async(standart_main, n, k, d, i * 1. / 4, max_word_num));
    }
    for (int i = 0; i < results.size(); i++) {
        results[i].wait();
    }
    std::cout << "[";
    for (int i = 0; i < x.size(); i++) {
        std::cout << x[i];
        if (i + 1 != x.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
    std::cout << "[";
    for (int i = 0; i < results.size(); i++) {
        std::cout << results[i].get();
        if (i + 1 != x.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]";
    return 0;
}
