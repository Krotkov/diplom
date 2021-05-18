#pragma once

#include <Decoder/Decoder.h>
#include <Matrix/Matrix.h>
#include <map>

class SCBase : public Decoder {
public:
    virtual Message calculateL(const MessageG &message, int i, const Channel &channel, const std::vector<bool> &flip,
                               std::pair<Message, std::vector<double>> &decoderAnswer, bool calcZMode = false) const;

    virtual double
    calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const = 0;

public:
    [[nodiscard]] virtual std::vector<double> calcZ(const Channel &channel, int iters = 100000) const;

    Message decode(const MessageG &message, const Channel &channel) const override;

    virtual ~SCBase() = default;

protected:
    int n_;
    std::vector<bool> frozen_;
    std::map<int, std::vector<int>> dynamicFrozen_;
    Matrix kernel_;
};