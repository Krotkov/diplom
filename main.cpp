#include <iostream>
#include <getopt.h>
#include "Channel/Channel.h"
#include "Code/PolarCode.h"
#include "utils/utils.h"

int main(int argc, char* argv[]) {
    int n = 8,  k = 4;
    double  noise = 1, erasure = 0.5;
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
        error_count += compareWords(a, b);

//        std::cout << error_count << "\n";

        word_error = (error_count * 1.0 / (i + 1));
        if (word_error > max_word_error) {
            std::cout << "number of words: " << (i+1) << "\n";
            std::cout << "errors on code word: " << word_error << "\n";
            return 0;
        }
    }
    std::cout << "number of words: " << max_word_num << "\n";
    std::cout << "errors on code word: " << word_error << "\n";
    return 0;
}
