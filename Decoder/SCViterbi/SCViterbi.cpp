//
// Created by kranya on 19.04.2021.
//

#include <Decoder/viterbi/Viterbi.h>
#include <iostream>
#include <cmath>
#include "SCViterbi.h"

Message SCViterbi::decode(const Message &message, const Channel &channel) const {
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

SCViterbi::SCViterbi(const PolarCodeWithLargeKernel &code) {
    n_ = code.getN();
    frozen_ = code.getFrozen();
    kernel_ = code.getKernel();

    auto curKernel = Matrix(kernel_.getN(), kernel_.getK() + 1);
    for (int i = 0; i < curKernel.getN(); i++) {
        auto row = kernel_[i];
        row.emplace_back(0);
        curKernel[i] = row;
    }
    for (int i = 0; i < kernel_.getN(); i++) {
        curKernel[0][curKernel.getK() - 1] = 1;
        viterbiVect_.emplace_back(curKernel);
        curKernel.removeRow(0);
    }
}

double
SCViterbi::calculateL(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel,
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
    ys.add(0);

    Message a;
    for (int j = 0; j < n_; j++) {
        a.add(0);
    }
    Message cur_ys = ys;

    for (int j = 0; j < (pref + i) % m; j++) {
        a += kernel_.getRow(j) * u[i - (pref + i) % m + j].get();
    }
    for (int j = 0; j + 1 < ys.size(); j++) {
        cur_ys[j] = ys[j].get() * (a[j] == 1 ? -1 : 1);
    }

    l_[n][pref + i] = viterbiVect_[(pref + i) % m].calcLLr(cur_ys, channel, (pref + i) % m);

//    for (int j = (pref + i) % m; j < m; j++) {
//        int ind = (pref + i) - ((pref + i) % m) + j;
//        l_[n][ind] = viterbiVect_[j].calcLLr(cur_ys, channel, j);
//        Symbol val;
//        if (l_[n][ind] > 0) {
//            val = 0;
//        } else {
//            val = 1;
//        }
//        auto w = Message();
//        w.add(val);
//        a += w * kernel_.getRow(j);
//        for (int q = 0; q + 1 < ys.size(); q++) {
//            cur_ys[q] = ys[q].get() * (a[q] == 1 ? -1 : 1);
//        }
//    }
    return l_[n][pref + i];
}
