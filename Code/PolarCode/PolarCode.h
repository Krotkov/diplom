#pragma once

#include <Message/Message.h>
#include <Matrix/Matrix.h>
#include "Code/Code.h"

class PolarCode : public Code {
public:
    PolarCode() = default;

    PolarCode(int n, int k, double err);

    PolarCode(int n, int k, const Matrix &kernel);

    Message encode(const Message &message) const override;

    int getN() const override;

    int getK() const override;

    Matrix getG() const override;

    virtual Matrix getFullG() const;

    virtual std::vector<bool> getFrozen() const;

    virtual Matrix getKernel() const;

    virtual ~PolarCode() = default;

protected:
    void constructCode(int n, int k);

    double calculateZ(int n, int i, double err) const;

    virtual Matrix calcR(const Matrix &a, int n) const;


    virtual Matrix calcBn(int n, int base) const;

    int n_;
    int k_;
    Matrix g_;
    std::vector<bool> frozen_;
    Matrix kernel_;
};