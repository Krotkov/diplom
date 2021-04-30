//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include <iostream>
#include "PolarCodeWithArikan.h"
#include "utils/utils.h"

void PolarCodeWithArikan::constructCode(int n, int k, double err) {
    n_ = n;
    k_ = k;

    std::vector<std::pair<double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {calculateZ(n, i + 1, err), i};
    }
    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n_, false);
    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }

    kernel_ = Matrix(2, 2);
    kernel_[0][0] = 1;
    kernel_[0][1] = 0;
    kernel_[1][0] = 1;
    kernel_[1][1] = 1;

    auto b = calcBn(n, 2);
    g_ = dot(calcBn(n, 2), kronPower(kernel_, getLog(n)));
}

double PolarCodeWithArikan::calculateZ(int n, int i, double err) const {
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

PolarCodeWithArikan::PolarCodeWithArikan(int n, int k, double err) {
    constructCode(n, k, err);
}
