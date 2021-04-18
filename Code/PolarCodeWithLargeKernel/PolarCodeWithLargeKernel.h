#pragma once

#include <Code/Code.h>

class PolarCodeWithLargeKernel : public Code {
public:
    PolarCodeWithLargeKernel(int n, int k, double err, const Matrix& kernel);

    void constructCode(int n, int k, double err, const Matrix& kernel);
};