//
// Created by kranya on 23.03.2021.
//

#include <cmath>
#include "SC.h"
#include "utils/utils.h"

SC::SC(const PolarCodeWithArikan &code) {
    frozen_ = code.getFrozen();
    n_ = code.getN();
    kernel_ = code.getKernel();
}

double
SC::calculateLStep(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel,
                   int n, int i, int pref) const {
    double value1 = y[1].get(), value2 = y[0].get();

    if (i % 2 == 0) {
        double value = std::min(std::abs(value1), std::abs(value2));
        l_[n][pref + i] = (value1 * value2 > 0) ? value : -value;
    } else {
        if (u[i - 1].get() == 0) {
            l_[n][pref + i] = value1 + value2;
        } else {
            l_[n][pref + i] = value1 - value2;
        }
    }
    return l_[n][pref + i];
}
