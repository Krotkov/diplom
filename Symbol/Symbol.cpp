//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include "Symbol.h"

Symbol operator+(const Symbol &a, const Symbol &b) {
    return Symbol((int)(a.value_ + b.value_) % 2);
}

Symbol operator*(const Symbol &a, const Symbol &b) {
    return Symbol((int)(a.value_ * b.value_) % 2);
}

Symbol &Symbol::operator+=(const Symbol &other) {
    value_ = (int)(value_ + other.value_) % 2;
    return *this;
}

Symbol &Symbol::operator*=(const Symbol &other) {
    value_ = (int)(value_ * other.value_) % 2;
    return *this;
}

void Symbol::addNoise(double noise) {
    if (value_ == 0) {
        value_ = 1.0 + noise;
    } else {
        value_ = noise - 1.0;
    }
}

bool operator==(const Symbol &a, const Symbol &b) {
    return a.value_ == b.value_;
}

double Symbol::get() const {
    return value_;
}

bool operator!=(const Symbol &a, const Symbol &b) {
    return !(a == b);
}

bool Symbol::operator<(const Symbol &other) const {
    return value_ < other.value_;
}
