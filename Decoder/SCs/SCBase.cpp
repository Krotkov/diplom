//
// Created by kranya on 30.04.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCBase.h"

double
SCBase::calculateL(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel,
                   int n, int i, int pref) const {
    if (n == 0) {
        l_[n][pref + i] = channel.getLLR(y[pref + i]);
        return l_[n][pref + i];
    }
    if (!std::isnan(l_[n][pref + i])) {
        return l_[n][pref + i];
    }

    int m = kernel_.getN();

    std::vector<Message> subU(m);
    std::vector<int> prefs(m);

    for (int j = 0; j < u.size(); j++) {
        subU[j % m].add(u[j]);
    }
    for (int j = 0; j < m; j++) {
        prefs[j] = pref + j * pow(m, n - 1);
    }

    Message ys;
    for (int j = 0; j < m; j++) {
        if (std::isnan(l_[n - 1][prefs[j] + i / m])) {
            Message new_u;
            for (int q = 0; q < m; q++) {
                if (kernel_[q][j] == 1) {
                    new_u += subU[q];
                }
            }
            ys.add(calculateL(l_, y, new_u, channel, n - 1, i / m, prefs[j]));
        } else {
            ys.add(l_[n - 1][prefs[j] + i / m]);
        }
    }
    calculateLStep(l_, ys, u, channel, n, i, pref);
    return l_[n][pref + i];
}

std::vector<double> SCBase::calcZ(const Channel &channel, int iters) const {
    int ln = getLog(n_, kernel_.size());
    std::vector<std::vector<double>> l_;
    l_.resize(ln + 1);

    for (int i = 0; i < ln + 1; i++) {
        l_[i].resize(n_, NAN);
    }

    std::vector<double> ans(n_);
    Message coded;
    for (int i = 0; i < n_; i++) {
        coded.add(0);
    }

    for (int j = 0; j < iters; j++) {
        if (j % 100 == 0) {
            std::cout << j << "\n";
        }
        auto message = channel.runMessage(coded);
        Message decoded;
        for (int i = 0; i < n_; i++) {
            double value = calculateL(l_, message, decoded, channel, ln, i);
            decoded.add(0);
            if (value > 0) {
                continue;
            } else {
                ans[i] += 1;
            }
        }
        for (int i = 0; i < ln + 1; i++) {
            for (int q = 0; q < n_; q++) {
                l_[i][q] = NAN;
            }
        }
    }
    for (int j = 0; j < n_; j++) {
        ans[j] /= iters;
    }

    return ans;
}

Message SCBase::decode(const Message &message, const Channel &channel) const {
    int ln = getLog(n_, kernel_.size());
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

    Message ans;
    for (int i = 0; i < decoded.size(); i++) {
        if (!frozen_[i]) {
            ans.add(decoded[i]);
        }
    }
    return ans;
}
