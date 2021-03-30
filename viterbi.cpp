//
// Created by kranya on 23.03.2021.
//

#include <utils/utils.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode.h>
#include <iostream>
#include "Decoder/viterbi/Viterbi.h"

int main() {
    int n = 4, k = 1;
    double erasure = 0.5, noise = 100;
    PolarCode code{n, k, erasure, noise};
    Viterbi viterbi(code.getG());
    PerfectGauss channel;
    int errorCount = 0;
    for (int i = 0; i < 1000; i++) {
        std::cout << i << "\n";
        Message a = generateWord(k);
        auto b = code.encode(a);
        b = channel.runMessage(b);
        auto d = viterbi.decode(b, channel);

        auto frozen = code.getFrozen();
        for (int j = 0; j < d.size(); j++) {
            if (!frozen[j] && a[j] != d[j]) {
                errorCount++;
                break;
            }
        }
    }
    std::cout << errorCount * 1.0 / 1000 << "\n";
//    d.print();
    return 0;
}