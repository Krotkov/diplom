#pragma once

#include <vector>
#include "../Matrix/Matrix.h"

class PolarCode {
public:
    PolarCode(int n, int k, double err, double noise);

    void constructCode(int n, int k, double err, double noise);

    Message encode(const Message& message) const;

    Message decode(const Message& message);

private:
    double calculateL(const Message& y, const Message& u, int n, int i, int pref=0);

    double calculateZ(int n, int i, double err) const;

    Matrix calcR(const Matrix &a) const;

    Matrix calcBn(int n) const;


    std::vector<std::vector<double> > l_;

    int n_;
    int k_;
    double err_;
    double noise_;
    std::vector<bool> frozen_;
    Matrix g_;
};