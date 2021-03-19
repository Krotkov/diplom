#pragma once

#include <random>
#include "../Message/Message.h"

class Channel {
private:
    const double E;
    std::random_device rd;
    mutable std::mt19937 gen;

public:
    explicit Channel(double E) : E(E), gen(rd()) {};

    double sigma(int n, int k) const;

    Message runMessage(Message& message, int n, int k);
};