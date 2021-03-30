#pragma once

#include <random>
#include <Channel/Channel.h>
#include "Message/Message.h"

class GausChannel : public Channel {
private:
    std::random_device rd;
    mutable std::mt19937 gen;
    double sigma_;
public:
    explicit GausChannel(int n, int k, double e) : gen(rd()) {
        sigma_ = 0.5 * std::pow(10, -e / 10) * ((double) n / k);
    };

    Message runMessage(const Message& message) const override;

    double getLLR(const Symbol& symbol) const override;
};