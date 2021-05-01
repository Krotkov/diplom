//
// Created by kranya on 05.03.2021.
//

#include "GausChannel.h"

MessageG GausChannel::runMessage(const Message &message) const {
    const double mySigma = sqrt(sigma_);
    std::normal_distribution<> d{0, mySigma};
    MessageG res;
    for (int i = 0; i < message.size(); i++) {
        auto noise = d(gen);
        if (message[i] == 0) {
            res.add(1.0 + noise);
        } else {
            res.add(noise - 1.0);
        }
    }
    return res;
}

double GausChannel::getLLR(const double &symbol) const {
    return 2 * symbol / sigma_;
}

void GausChannel::setNoise(double noise) {
    sigma_ = 0.5 * std::pow(10, -noise / 10) * ((double) n_ / k_);
}
