//
// Created by kranya on 31.03.2021.
//

#include <cmath>
#include "PerfectGauss.h"

Message PerfectGauss::runMessage(const Message &message) const {
    Message ans;
    for (int i = 0; i < message.size(); i++) {
        if (message[i] == 0) {
            ans.add(1);
        } else {
            ans.add(-1);
        }
    }
    return ans;
}

double PerfectGauss::getLLR(const Symbol &symbol) const {
    if (symbol == 1) {
        return std::log(0.01 / 0.99);
    } else {
        return std::log(0.99 / 0.01);
    }
}
