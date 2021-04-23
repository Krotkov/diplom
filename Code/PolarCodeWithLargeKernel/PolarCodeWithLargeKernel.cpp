//
// Created by kranya on 19.04.2021.
//

#include <set>
#include <Channel/Gaus/GausChannel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include "PolarCodeWithLargeKernel.h"

PolarCodeWithLargeKernel::PolarCodeWithLargeKernel(int n, int k, double err, const Matrix &kernel) {
    constructCode(n, k, err, kernel);
}

void PolarCodeWithLargeKernel::constructCode(int n, int k, double err, const Matrix &kernel) {
    n_ = n;
    k_ = k;
    kernel_ = kernel;

    auto b = calcBn(n, kernel_.size());
    g_ = dot(calcBn(n, kernel_.size()), kronPower(kernel_, getLog(n, kernel_.size())));

    GausChannel channel(n_, n_, 0);
    SCViterbi viterbi(*this);
    auto z = viterbi.calcZ(channel, 10000);

    std::vector<std::pair<long double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {z[i], i};
    }

    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n_, false);
    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }
}
