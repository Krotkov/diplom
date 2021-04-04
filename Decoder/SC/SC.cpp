//
// Created by kranya on 23.03.2021.
//

#include <cmath>
#include "SC.h"
#include "utils/utils.h"

SC::SC(const PolarCode &code) {
    frozen_ = code.getFrozen();
    n_ = code.getN();
}

double
SC::calculateL(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel, int n,
               int i, int pref) const {
    if (n == 0) {
        l_[n][pref + i] = channel.getLLR(y[0]);
        return l_[n][pref + i];
    }
    if (!std::isnan(l_[n][pref + i])) {
        return l_[n][pref + i];
    }
    int next_pref_1 = pref;
    int next_pref_2 = pref + (1 << (n - 1));
    long double value1, value2;

    if (std::isnan(l_[n - 1][next_pref_1 + (i) / 2])) {
        Message new_y;
        for (int j = y.size() / 2; j < y.size(); j++) {
            new_y.add(y[j]);
        }
        Message new_u;
        for (int j = 1; j < u.size(); j += 2) {
            new_u.add(u[j]);
        }
        value1 = calculateL(l_, new_y, new_u, channel, n - 1, (i) / 2, next_pref_1);
    } else {
        value1 = l_[n - 1][next_pref_1 + (i) / 2];
    }
    if (std::isnan(l_[n - 1][next_pref_2 + (i) / 2])) {
        Message new_y;
        for (int j = 0; j < y.size() / 2; j++) {
            new_y.add(y[j]);
        }
        Message new_u;
        for (int j = 0; j < u.size(); j += 2) {
            if (j + 1 != u.size()) {
                new_u.add(u[j] + u[j + 1]);
            }
        }
        value2 = calculateL(l_, new_y, new_u, channel, n - 1, (i) / 2, next_pref_2);
    } else {
        value2 = l_[n - 1][next_pref_2 + (i) / 2];
    }
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

Message SC::decode(const Message &message, const Channel &channel) const {
    int ln = getLog(n_);
    std::vector<std::vector<double>> l_;
    l_.resize(ln + 1);

    for (int i = 0; i < ln + 1; i++) {
        l_[i].resize(n_, NAN);
    }
    Message decoded;
    for (int i = 0; i < message.size(); i++) {
        if (frozen_[i]) {
            decoded.add(0);
        } else {
            double value = calculateL(l_, message, decoded, channel, ln, i);
            if (value > 0) {
                decoded.add(0);
            } else {
                decoded.add(1);
            }
        }
    }

    return decoded;
}
