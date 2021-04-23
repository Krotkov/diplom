#pragma once

#include <vector>
#include <cassert>
#include <Symbol/Symbol.h>

class Message : public std::vector<Symbol> {
public:
    Message() : vector() {};

    void add(const Symbol &s);

    void print();

    friend int compare(const Message &a, const Message &b);

    friend Message operator+(const Message &a, const Message &b);

    friend Message operator*(const Message &a, const Message &b);

    friend Message operator*(const Message &a, double val);

    Message &operator+=(const Message &other);

    Message &operator*=(const Message &other);

    Message &operator*=(double val);

    int getWeight() const;
};

int compare(const Message &a, const Message &b);

Message operator+(const Message &a, const Message &b);

Message operator*(const Message &a, const Message &b);

Message operator*(const Message &a, double val);