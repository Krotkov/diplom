//
// Created by kranya on 31.03.2021.
//

#include <cmath>
#include "PerfectGauss.h"

MessageG PerfectGauss::runMessage(const Message &message) const {
    MessageG ans;
    for (int i = 0; i < message.size(); i++) {
        if (message[i] == 0) {
            ans.add(1);
        } else {
            ans.add(-1);
        }
    }
    return ans;
}

double PerfectGauss::getLLR(const double &symbol) const {
    if (symbol == 1) {
        return std::log(0.99 / 0.01);
    } else if (symbol == 0) {
        //special for scviterbi
        return 0;
    } else {
        return std::log(0.01 / 0.99);
    }
}

PerfectGauss::PerfectGauss() : GausChannel(1, 1, 1) {}
