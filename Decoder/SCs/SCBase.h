#pragma once

#include <Decoder/Decoder.h>
#include <Matrix/Matrix.h>
#include <Code/PolarCode/PolarCode.h>
#include <map>

class SCBase : public Decoder {
public:
    SCBase(const PolarCode &code);

    virtual Message calculateL(const MessageG &message, int n, int i, const Channel &channel, const std::vector<bool> &flip,
                               std::pair<Message, std::vector<double>> &decoderAnswer, bool calcZMode = false) const;

    virtual double
    calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const = 0;

public:
    void recursiveSpecialNodesCalc(int n, int i, int l, int r);

    [[nodiscard]] virtual std::vector<double> calcZ(const Channel &channel, int iters = 100000) const;

    Message decode(const MessageG &message, const Channel &channel) const override;

    virtual ~SCBase() = default;

    enum Special {NONE, RATE0, REP, SPC, RATE1};

protected:
    int n_;
    std::vector<bool> frozen_;
    std::vector<std::vector<Special>> specialNodes_;
    Matrix kernel_;
};