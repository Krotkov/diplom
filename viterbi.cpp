//
// Created by kranya on 23.03.2021.
//

#include <iostream>
#include <utils/utils.h>
#include "Decoder/SC/SC.h"
#include "Decoder/viterbi/Viterbi.h"

int main() {
//    int n = 2, k = 1;
//    double erasure = 0.5, noise=1;
//    PolarCode code{n, k, erasure, noise};
//    Channel channel;
//
//    Message a = generateWord(k);
//    auto b = code.encode(a);
//    b.print();
//    auto c = channel.runMessage(b, n, k, noise);
//    c.print();
//    Viterbi viterbi(code.getG(), n, k, noise);
//    auto d = viterbi.decode(c);
//
//    d.print();

    int n = 3, k = 1;
    double noise = 30;
    Matrix g(2, 3);
    g[0][0] = 1;
    g[0][1] = 1;
    g[0][2] = 0;
    g[1][0] = 0;
    g[1][1] = 1;
    g[1][2] = 1;

    Message word;
    word.add(0);
    word.add(1);
    word.add(1);

    Channel channel;
    Viterbi viterbi(g, n, k, noise);

    auto w1 = channel.runMessage(word, n, k, noise);

    for (int i = 0; i < w1.size(); i++) {
        if (word[i] == 0) {
            w1[i] = 1;
        } else {
            w1[i] = -1;
        }
    }

    auto decoded = viterbi.decode(w1);

    decoded.print();
    return 0;
}