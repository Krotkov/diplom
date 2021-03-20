//
// Created by kranya on 05.03.2021.
//

#include "Channel.h"

Message Channel::runMessage(Message &message, int n, int k, double E) {
    const double mySigma = sqrt(sigma(n, k, E));
    std::normal_distribution<> d{0, mySigma};
    auto res = Message(message);
    for (int i = 0; i < res.size(); i++) {
        auto noise = d(gen);
        res[i].addNoise(noise);
    }
    return res;
}

double Channel::sigma(int n, int k, double E) {
    return 0.5 * pow(10, -E / 10) * ((double) n / k);
}
