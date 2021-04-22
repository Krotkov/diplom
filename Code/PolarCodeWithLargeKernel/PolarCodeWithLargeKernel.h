#pragma once

#include <Code/Code.h>

class PolarCodeWithLargeKernel : public Code {
public:
    PolarCodeWithLargeKernel(int n, int k, double err, const Matrix& kernel);

    void constructCode(int n, int k, double err, const Matrix& kernel);

    double calcZ(int n, int i, double err, int kernelSize) const;

    double calcQ(double err, int nu) const;
private:
    void calcRn(std::vector<int>& x, int begin, int end) const;

    std::vector<std::vector<int>> a_;
    std::vector<int> d_;
};