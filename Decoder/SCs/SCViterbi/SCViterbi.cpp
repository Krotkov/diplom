//
// Created by kranya on 19.04.2021.
//

#include <Decoder/viterbi/Viterbi.h>
#include "SCViterbi.h"

SCViterbi::SCViterbi(const PolarCode &code) : SCBase(code) {
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

double SCViterbi::calculateLStep(const MessageG &y, const Message &u,
                                 const Channel &channel) const {
    MessageG ys = y;
    ys.add(0);

    Message a;
    for (int j = 0; j < n_; j++) {
        a.add(0);
    }
    MessageG cur_ys = ys;

    for (int j = 0; j < u.size(); j++) {
        a += kernel_.getRow(j) * u[j].get();
    }
    for (int j = 0; j + 1 < ys.size(); j++) {
        cur_ys[j] = ys[j] * (a[j] == 1 ? -1 : 1);
    }

    return viterbiVect_[u.size()].calcLLr(cur_ys, channel);
}

