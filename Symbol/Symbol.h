#pragma once

#include <ostream>

class Symbol {
public:
    Symbol() : value_(0) {};

    Symbol(int v) {
        if (v < 0) {
            value_ = v;
        } else {
            value_ = v % 2;
        }
    };

    Symbol &operator=(const Symbol &other);

    friend Symbol operator+(const Symbol& a, const Symbol& b);

    friend Symbol operator*(const Symbol& a, const Symbol& b);

    friend bool operator==(const Symbol& a, const Symbol& b);

    friend bool operator!=(const Symbol& a, const Symbol& b);

    friend std::ostream& operator<<(std::ostream& os, const Symbol& symbol);

    bool operator<(const Symbol& other) const;

    Symbol& operator+=(const Symbol& other);

    Symbol& operator*=(const Symbol& other);

    int get() const;
private:
    int value_;
};

Symbol operator+(const Symbol& a, const Symbol& b);
Symbol operator*(const Symbol& a, const Symbol& b);
bool operator==(const Symbol& a, const Symbol& b);
bool operator!=(const Symbol& a, const Symbol& b);

std::ostream& operator<<(std::ostream& os, const Symbol& symbol);