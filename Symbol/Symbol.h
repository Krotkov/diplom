#pragma once

class Symbol {
public:
    Symbol() : value_(0) {};

    Symbol(double v) : value_(v) {};

    friend Symbol operator+(const Symbol& a, const Symbol& b);

    friend Symbol operator*(const Symbol& a, const Symbol& b);

    Symbol& operator+=(const Symbol& other);

    Symbol& operator*=(const Symbol& other);

    void addNoise(double noise);
private:
    double value_;
};

Symbol operator+(const Symbol& a, const Symbol& b);
Symbol operator*(const Symbol& a, const Symbol& b);