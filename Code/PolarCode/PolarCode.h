#pragma once

#include <vector>
#include "Matrix/Matrix.h"
#include "Message/Message.h"

class PolarCode {
public:
    PolarCode(int n, int k, double err);

    void constructCode(int n, int k, double err);

    Message encode(const Message& message) const;

    std::vector<bool> getFrozen() const;

    int getN() const;
    int getK() const;

    Matrix getG() const;

private:

    double calculateZ(int n, int i, double err) const;

    Matrix calcR(const Matrix &a) const;

    Matrix calcBn(int n) const;


    int n_;
    int k_;
protected:
    Matrix g_;
    std::vector<bool> frozen_;
};