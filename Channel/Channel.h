#pragma once

#include <random>
#include "../Message/Message.h"

class Channel {
private:
    std::random_device rd;
    mutable std::mt19937 gen;

public:
    explicit Channel() : gen(rd()) {};

    static double sigma(int n, int k, double E);

    Message runMessage(Message& message, int n, int k, double E);
};