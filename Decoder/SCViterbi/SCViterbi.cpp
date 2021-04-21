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
        l_[n][pref + i] = channel.getLLR(y[pref+i]);
        return l_[n][pref + i];
    }
    if (!std::isnan(l_[n][pref + i])) {
        return l_[n][pref + i];
    }

    int m = kernel_.getN();

    std::vector<Message> subU(m);
    for (int j = 0; j < u.size(); j++) {
        subU[j%m].add(u[j]);
    }
    std::vector<int> prefs(m);
    for (int j = 0; j < m; j++) {
        prefs[j] = pref + j * pow(m, n-1);
    }

    Message ys;
    for (int j = 0; j < m; j++) {
        Message new_u;
        for (int q = 0; q < m; q++) {
            if (kernel_[j][q] == 1) {
                new_u += subU[q];
            }
        }
        ys.add(calculateL(l_, y, new_u, channel, n-1, i / m, prefs[j]));
    }
    ys.add(0);

    for (int j = (pref+i)%m; j < m; j++) {
        int ind = (pref+i)-((pref+i)%m) + j;
        l_[n][ind] = viterbiVect_[j].calcLLr(ys, channel, j);
    }

//    l_[n][pref+i] = viterbiVect_[(pref+i)%m].calcLLr(ys, channel, (pref+i)%m);
    return l_[n][pref+i];
}
