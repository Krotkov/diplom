//
// Created by kranya on 23.03.2021.
//

#include <utils/utils.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode/PolarCode.h>
#include <iostream>
#include <Channel/Gaus/GausChannel.h>
#include "Decoder/viterbi/Viterbi.h"

int main() {
    int n = 8, k = 4;
    double erasure = 0.5, noise = 1;
    PolarCode code{n, k, erasure};
    Viterbi viterbi(code.getG());
    GausChannel channel(n, k, noise);
    int errorCount = 0;
    for (int i = 0; i < 1000; i++) {
        std::cout << i << "\n";
        Message a = generateWord(k);
        auto b = code.encode(a);
        auto c = channel.runMessage(b);
        auto d = viterbi.decode(c, channel);

        auto frozen = code.getFrozen();
        for (int j = 0; j < d.size(); j++) {
            if (!frozen[j] && b[j] != d[j]) {
                errorCount++;
                break;
            }
        }
    }
    std::cout << errorCount * 1.0 / 1000 << "\n";
//    d.print();
    return 0;
}