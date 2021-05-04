//
// Created by kranya on 19.04.2021.
//

#include <Decoder/viterbi/Viterbi.h>
#include "SCViterbi.h"

SCViterbi::SCViterbi(const PolarCode &code) {
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

double SCViterbi::calculateLStep(std::vector<std::vector<double>> &l_, const MessageG &y, const Message &u,
                                 const Channel &channel, int n, int i, int pref) const {
    MessageG ys = y;
    ys.add(0);

    int m = kernel_.size();

    Message a;
    for (int j = 0; j < n_; j++) {
        a.add(0);
    }
    MessageG cur_ys = ys;

    for (int j = 0; j < (pref + i) % m; j++) {
        a += kernel_.getRow(j) * u[j].get();
    }
    for (int j = 0; j + 1 < ys.size(); j++) {
        cur_ys[j] = ys[j] * (a[j] == 1 ? -1 : 1);
    }

    l_[n][pref + i] = viterbiVect_[(pref + i) % m].calcLLr(cur_ys, channel);

    return l_[n][pref + i];
}

