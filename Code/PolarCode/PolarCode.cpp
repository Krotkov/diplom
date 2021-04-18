//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include "PolarCode.h"
#include "utils/utils.h"

void PolarCode::constructCode(int n, int k, double err) {
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

    Matrix f = Matrix(2, 2);
    f[0][0] = 1;
    f[0][1] = 0;
    f[1][0] = 1;
    f[1][1] = 1;

    auto b = calcBn(n);
    g_ = dot(calcBn(n), kronPower(f, getLog(n)));
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

Message PolarCode::encode(const Message &message) const {
    assert(message.size() == k_);

    Message expanded;

    int ind = 0;
    for (int i = 0; i < n_; i++) {
        if (frozen_[i]) {
            expanded.add(0);
        } else {
            expanded.add(message[ind]);
            ind++;
        }
    }

    return dot(Matrix(expanded), g_).getRow(0);
}
