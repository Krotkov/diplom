//
// Created by kranya on 30.04.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCBase.h"

double
SCBase::calculateL(std::vector<std::vector<double>> &l_, std::vector<std::vector<Symbol>> &us, const MessageG &y,
                   const Channel &channel,
                   int n, int i, int pref) const {
    if (n == 0) {
        l_[n][pref + i] = channel.getLLR(y[pref + i]);
        return l_[n][pref + i];
    }
    if (!std::isnan(l_[n][pref + i])) {
        return l_[n][pref + i];
    }

    int m = kernel_.getN();

    MessageG ys;
    for (int j = 0; j < m; j++) {
        int new_pref = pref + j * pow(m, n - 1);
        if (std::isnan(l_[n - 1][new_pref + i / m])) {
            ys.add(calculateL(l_, us, y, channel, n - 1, i / m, new_pref));
        } else {
            ys.add(l_[n - 1][new_pref + i / m]);
        }
    }

    Message u;
    for (int j = 0; j < (pref + i) % m; j++) {
        u.add(us[n][pref + i - j]);
    }
    std::reverse(u.begin(), u.end());

    calculateLStep(l_, ys, u, channel, n, i, pref);
    return l_[n][pref + i];
}

std::vector<double> SCBase::calcZ(const Channel &channel, int iters) const {
    int ln = getLog(n_, kernel_.size());
    std::vector<std::vector<double>> l_;
    std::vector<std::vector<Symbol>> us;
    l_.resize(ln + 1);
    us.resize(ln + 1);
    for (int i = 0; i < ln + 1; i++) {
        l_[i].resize(n_, NAN);
        us[i].resize(n_, Symbol(-1));
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
            double value = calculateL(l_, us, message, channel, ln, i);
            decoded.add(0);
            if (value <= 0) {
                ans[i] += 1;
            }
            if (i != n_ - 1) {
                updateUs(us, ln, 0, i + 1, decoded.back());
            }
        }
        for (int i = 0; i < ln + 1; i++) {
            for (int q = 0; q < n_; q++) {
                l_[i][q] = NAN;
                us[i][q] = Symbol(-1);
            }
        }
    }
    for (int j = 0; j < n_; j++) {
        ans[j] /= iters;
    }

    return ans;
}

Message SCBase::decode(const MessageG &message, const Channel &channel) const {
    int ln = getLog(n_, kernel_.size());
    std::vector<std::vector<double>> l_;
    std::vector<std::vector<Symbol>> us;
    l_.resize(ln + 1);
    us.resize(ln + 1);
    for (int i = 0; i < ln + 1; i++) {
        l_[i].resize(n_, NAN);
        us[i].resize(n_, Symbol(-1));
    }

    Message decoded;
    for (int i = 0; i < message.size(); i++) {
        if (frozen_[i]) {
            decoded.add(0);
        } else {
            double value = calculateL(l_, us, message, channel, ln, i);
            if (value > 0) {
                decoded.add(0);
            } else {
                decoded.add(1);
            }
        }
        if (i != n_ - 1) {
            updateUs(us, ln, 0, i + 1, decoded.back());
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

void SCBase::updateUs(std::vector<std::vector<Symbol>> &us, int n, int pref, int i, const Symbol &symbol) const {
    if (n == 0 || us[n][pref+i] != -1) {
        return;
    }
    us[n][pref+i] = symbol;
    Message message;
    if (i % kernel_.getN() == 0 && pref+i != 0) {

        for (int j = pref+i - kernel_.getN() + 1; j <= pref+i; j++) {
            message.add(us[n][j]);
        }
        Message a;
        for (int j = 0; j < kernel_.getN(); j++) {
            a.add(0);
        }
        for (int j = 0; j < kernel_.getN(); j++) {
            for (int q = 0; q < kernel_.getK(); q++) {
                a[q] += message[j] * kernel_[j][q];
            }
        }
        for (int j = 0; j < kernel_.getN(); j++) {
            int new_pref = pref + j * pow(kernel_.getN(), n - 1);
            updateUs(us, n - 1, new_pref, i / kernel_.getN(), a[j]);
        }
    }
}
