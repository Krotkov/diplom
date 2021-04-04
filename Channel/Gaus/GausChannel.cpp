//
// Created by kranya on 05.03.2021.
//

#include "GausChannel.h"

Message GausChannel::runMessage(const Message &message) const {
    const double mySigma = sqrt(sigma_);
    std::normal_distribution<> d{0, mySigma};
    auto res = Message(message);
    for (int i = 0; i < res.size(); i++) {
        auto noise = d(gen);
        res[i].addNoise(noise);
    }
    return res;
}

double GausChannel::getLLR(const Symbol &symbol) const {
    return 2 * symbol.get() / sigma_;
}

void GausChannel::setNoise(double noise) {
    sigma_ = 0.5 * std::pow(10, -noise / 10) * ((double) n_ / k_);
}
