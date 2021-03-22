#include <iostream>
#include <getopt.h>
#include <future>
#include "Channel/Channel.h"
#include "Code/PolarCode.h"
#include "utils/utils.h"

double standart_main(int n, int k, double noise, double erasure, int max_word_num, double max_word_error) {
    int error_count = 0;
    double word_error = 0;
    PolarCode code{n, k, erasure, noise};
    Channel channel;
    Message a, b;
    for (int i = 0; i < max_word_num; i++) {
        a = generateWord(k);

        b = code.encode(a);
        b = channel.runMessage(b, n, k, noise);
        b = code.decode(b);
        if (compareWords(a, b) > 0) {
            error_count++;
        }
//        error_count += compareWords(a, b);

//        std::cout << error_count << "\n";

//        word_error = (error_count * 1.0 / (i + 1));
//        if (word_error > max_word_error) {
//            std::cout << "number of words: " << (i+1) << "\n";
//            std::cout << "errors on code word: " << word_error << "\n";
//            return;
//        }
    }
    return (error_count * 1.0) / max_word_num;
//    std::cout << "number of words: " << max_word_num << "\n";
//    std::cout << "errors on code word: " << (error_count * 1.0) / max_word_error << "\n";
}

void graph_values(int n, int k, double noise, double erasure, int max_word_num, double max_word_error) {
    PolarCode code{n, k, erasure, noise};
    Channel channel;
    Message a, b;
    std::vector<double> x, y;
    for (int i = 1; i <= 16; i++) {
        std::cout << i << "\n";
        x.push_back(i * 1.0);
        code.setNoise((i * 1.0));
        int error_count = 0;
        for (int j = 0; j < max_word_num; j++) {
            if (j % 1000 == 0) {
                std::cout << "-" << j << "\n";
            }
            a = generateWord(k);

            b = code.encode(a);
            b = channel.runMessage(b, n, k, (i * 1.0));
            b = code.decode(b);
            if (compareWords(a, b) > 0) {
                error_count++;
            }
        }
        y.push_back(error_count * 1.0 / max_word_num);
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
    for (int i = 0; i < x.size(); i++) {
        std::cout << y[i];
        if (i + 1 != y.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
}

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

    std::vector<std::future<double>> results;
    std::vector<double> x;
    results.reserve(20);
    for (int i = 3; i < 23; i++) {
        x.push_back((i + 1) * 1.0 / 4);
        results.push_back(std::async(standart_main, n, k, (i + 1) * 1.0 / 4, erasure, max_word_num, max_word_error));
    }
    for (int i = 0; i < 20; i++) {
        results[i].wait();
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
        std::cout << results[i].get();
        if (i != 19) {
            std::cout << ",";
        }
    }
    std::cout << "]";

//    graph_values(n, k, noise, erasure, max_word_num, max_word_error);
//    standart_main(n, k, noise, erasure, max_word_num, max_word_error);
    return 0;
}
