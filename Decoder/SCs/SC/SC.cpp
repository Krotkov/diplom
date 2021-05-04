//
// Created by kranya on 23.03.2021.
//

#include <cmath>
#include <Code/PolarCode/PolarCode.h>
#include "SC.h"
#include "utils/utils.h"

SC::SC(const PolarCode &code) {
    frozen_ = code.getFrozen();
    n_ = code.getN();
    kernel_ = code.getKernel();
}

double
SC::calculateLStep(std::vector<std::vector<double>> &l_, const MessageG &y, const Message &u, const Channel &channel,
                   int n, int i, int pref) const {
    double value1 = y[1], value2 = y[0];

    if (i % 2 == 0) {
        double value = std::min(std::abs(value1), std::abs(value2));
        l_[n][pref + i] = (value1 * value2 > 0) ? value : -value;
    } else {
        if (u.back().get() == 0) {
            l_[n][pref + i] = value1 + value2;
        } else {
            l_[n][pref + i] = value1 - value2;
        }
    }
    return l_[n][pref + i];
}

