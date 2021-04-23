//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include <iostream>
#include <Channel/Gaus/GausChannel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include "PolarCode.h"
#include "utils/utils.h"

void PolarCode::constructCode(int n, int k, double err) {
    n_ = n;
    k_ = k;

    kernel_ = Matrix(2, 2);
    kernel_[0][0] = 1;
    kernel_[0][1] = 0;
    kernel_[1][0] = 1;
    kernel_[1][1] = 1;

    auto b = calcBn(n, 2);
    g_ = dot(calcBn(n, 2), kronPower(kernel_, getLog(n)));

    GausChannel channel(n_, n_, 0);
    SCViterbi viterbi(*this);
    auto z = viterbi.calcZ(channel, 10000);

    std::vector<std::pair<double, int>> zz(n);

    for (int i = 0; i < n_; i++) {
        zz[i] = {z[i], i};
    }

    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n_, false);
    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }
}

double PolarCode::calculateZ(int n, int i, double err) const {
    if (n == 1 && i == 1) {
        return err;
    }
    if (i % 2 == 0) {
        auto z = calculateZ(n / 2, i / 2, err);
        return z * z;
    } else {
        auto z = calculateZ(n / 2, (i + 1) / 2, err);
        return 2 * z - z * z;
    }
}

PolarCode::PolarCode(int n, int k, double err) {
    constructCode(n, k, err);
}
