#pragma once

#include <Code/PolarCode.h>

class PolarCodeWithLargeKernel : public PolarCode {
public:
    PolarCodeWithLargeKernel(int n, int k, double err, const Matrix& kernel);

    void constructCode(int n, int k, double err, const Matrix& kernel);
};