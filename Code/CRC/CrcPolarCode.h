#pragma once

#include <Code/PolarCode/PolarCode.h>
#include <Polynom/Polynom.h>

class CrcPolarCode : public Code {
public:
    CrcPolarCode(int n, int k, int m = 16);

    CrcPolarCode(int n, int k, const Matrix &kernel, int m = 16);

    int getN() const override;

    int getK() const override;

    Matrix getG() const override;

    Message encode(const Message &message) const override;

    bool check(const Message &message) const;

private:
    void createPolynom(int m);

    PolarCode polarCode_;
    Polynom<int> crc_;
    int m_;
};