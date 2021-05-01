//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include <ostream>
#include "Symbol.h"

Symbol operator+(const Symbol &a, const Symbol &b) {
    return Symbol((a.value_ + b.value_) & 1);
}

Symbol operator*(const Symbol &a, const Symbol &b) {
    return Symbol((a.value_ * b.value_) & 1);
}

Symbol &Symbol::operator+=(const Symbol &other) {
    value_ = (value_ + other.value_) & 1;
    return *this;
}

Symbol &Symbol::operator*=(const Symbol &other) {
    value_ = (value_ * other.value_) & 1;
    return *this;
}

bool operator==(const Symbol &a, const Symbol &b) {
    return a.value_ == b.value_;
}

int Symbol::get() const {
    return value_;
}

bool operator!=(const Symbol &a, const Symbol &b) {
    return !(a == b);
}

bool Symbol::operator<(const Symbol &other) const {
    return value_ < other.value_;
}

std::ostream &operator<<(std::ostream &os, const Symbol &symbol) {
    os << symbol.value_;
    return os;
}

Symbol &Symbol::operator=(const Symbol &other) {
    (*this).value_ = other.value_;
    return (*this);
}
