//
// Created by kranya on 01.05.2021.
//

#include "CrcPolarCode.h"

int CrcPolarCode::getN() const {
    return polarCode_.getN();
}

int CrcPolarCode::getK() const {
    return polarCode_.getK() - m_;
}

Matrix CrcPolarCode::getG() const {
    return polarCode_.getG();
}

CrcPolarCode::CrcPolarCode(int n, int k, int m) {
    polarCode_ = PolarCode(n, k + m, 0.5);
    createPolynom(m);
    m_ = m;
}

CrcPolarCode::CrcPolarCode(int n, int k, const Matrix &kernel, int m) {
    createPolynom(m);
    m_ = m;
    polarCode_ = PolarCode(n, k + m, kernel);
}

void CrcPolarCode::createPolynom(int m) {
    if (m == 16) {
        crc_ = Polynom<int>(16);
        crc_[0] = 1;
        crc_[2] = 1;
        crc_[15] = 1;
        crc_[16] = 1;
    } else {
        throw std::runtime_error("can't create crc polynom");
    }
}

Message CrcPolarCode::encode(const Message &message) const {
    auto messagePol = polFromMessage(message);
    auto modPol = messagePol % crc_;
    Message expanded = message;
    int maxStep = modPol.maxStep();
    for (int i = 0; i < m_; i++) {
        if (i <= maxStep) {
            expanded.add(modPol[i]);
        } else {
            expanded.add(0);
        }
    }
    return polarCode_.encode(expanded);
}

bool CrcPolarCode::check(const Message &message) const {
    Message info, mod;
    for (int i = 0; i < getK(); i++) {
        info.add(message[i]);
    }
    for (int i = getK(); i < getK() + m_; i++) {
        mod.add(message[i]);
    }
    Polynom<int> infoPol = polFromMessage(info);
    Polynom<int> modPol = polFromMessage(mod);
    return ((infoPol % crc_) == modPol);
}

PolarCode CrcPolarCode::getPolarCode() const {
    return polarCode_;
}

int CrcPolarCode::getM() const {
    return m_;
}

std::vector<bool> CrcPolarCode::getFrozen() const {
    return polarCode_.getFrozen();
}
