#pragma once

#include <random>
#include <Channel/Channel.h>
#include "Message/Message.h"

class GausChannel : public Channel {
private:
    std::random_device rd;
    mutable std::mt19937 gen;
    double sigma_;
    int n_;
    int k_;
public:
    explicit GausChannel(int n, int k, double e) : gen(rd()) {
        n_ = n;
        k_ = k;
        setNoise(e);
    };

    GausChannel(const GausChannel& channel) : gen(rd()) {
        n_ = channel.n_;
        k_ = channel.k_;
        sigma_ = channel.sigma_;
    };

    Message runMessage(const Message& message) const override;

    double getLLR(const Symbol& symbol) const override;

    void setNoise(double noise);
};