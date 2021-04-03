#pragma once

#include <vector>
#include <Code/Code.h>
#include "Matrix/Matrix.h"
#include "Message/Message.h"

class PolarCode : public Code {
public:
    PolarCode(int n, int k, double err);

    void constructCode(int n, int k, double err);

    Message encode(const Message& message) const override;

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