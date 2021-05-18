//
// Created by kranya on 23.03.2021.
//

#include <cmath>
#include <Code/PolarCode/PolarCode.h>
#include "SC.h"
#include "utils/utils.h"

SC::SC(const PolarCode &code) : SCBase(code) {}

double
SC::calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const {
    double value1 = y[1], value2 = y[0];

    if (u.size() % 2 == 0) {
        double value = std::min(std::abs(value1), std::abs(value2));
        return (value1 * value2 > 0) ? value : -value;
    } else {
        if (u.back().get() == 0) {
            return value1 + value2;
        } else {
            return value1 - value2;
        }
    }
}

