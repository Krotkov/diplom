//
// Created by kranya on 23.03.2021.
//

#include "Decoder/SC/SC.h"
#include "Decoder/viterbi/Viterbi.h"

int main() {
    int n = 2, k = 1;
    double erasure = 0.5, noise=1;
    PolarCode code{n, k, erasure, noise};
    Channel channel;
    Message a, b;
    Viterbi decoder(code.getG());
    return 0;
}