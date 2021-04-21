//
// Created by kranya on 19.04.2021.
//

#include "PolarCodeWithLargeKernel.h"

PolarCodeWithLargeKernel::PolarCodeWithLargeKernel(int n, int k, double err, const Matrix& kernel) {
    constructCode(n, k, err, kernel);
}

void PolarCodeWithLargeKernel::constructCode(int n, int k, double err, const Matrix& kernel) {
    n_ = n;
    k_ = k;
    kernel_ = kernel;
    frozen_.resize(n, false);

    auto b = calcBn(n, kernel_.size());
    g_ = dot(calcBn(n, kernel_.size()), kronPower(kernel_, getLog(n, kernel_.size())));
}

