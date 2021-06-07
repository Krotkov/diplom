#pragma once

#include <Code/PolarCode/PolarCode.h>
#include <Polynom/Polynom.h>

class CrcPolarCode : public Code {
public:
    CrcPolarCode() = default;

    CrcPolarCode(int n, int k, int m = 16);

    CrcPolarCode(int n, int k, const Matrix &kernel, const std::map<int, std::vector<int>> &dynamicFrozen, int m = 16);

    CrcPolarCode(int n, int k, const Matrix &kernel, const Matrix &rKernel, int m = 16);

    int getN() const override;

    int getK() const override;

    int getM() const;

    std::vector<bool> getFrozen() const;

    Matrix getKernel() const;

    Message encode(const Message &message) const override;

    bool check(const Message &message) const;

    PolarCode getPolarCode() const;

private:
    void createPolynom(int m);

    PolarCode polarCode_;
    Polynom<int> crc_;
    int m_;
};