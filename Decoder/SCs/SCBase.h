#pragma once

#include <Decoder/Decoder.h>
#include <Matrix/Matrix.h>

class SCBase : public Decoder {
protected:
    virtual double
    calculateL(std::vector <std::vector<double>> &l_, const MessageG &y, const Message &u, const Channel &channel, int n,
               int i, int pref = 0) const;

    virtual double
    calculateLStep(std::vector <std::vector<double>> &l_, const MessageG &y, const Message &u, const Channel &channel,
                   int n,
                   int i, int pref = 0) const = 0;

public:
    [[nodiscard]] virtual std::vector<double> calcZ(const Channel& channel, int iters=100000) const;

    Message decode(const MessageG& message, const Channel& channel) const override;

    virtual ~SCBase() = default;
protected:
    int n_;
    std::vector<bool> frozen_;
    Matrix kernel_;
};