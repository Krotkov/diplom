#pragma once

#include <Decoder/Decoder.h>
#include <Matrix/Matrix.h>
#include <map>

class SCBase : public Decoder {
public:
    virtual double
    calculateL(std::vector<std::vector<double>> &l_, std::vector<std::vector<Symbol>> &us, const MessageG &y, const Channel &channel, int n,
               int i, int pref = 0) const;

    virtual double
    calculateLStep(std::vector<std::vector<double>> &l_, const MessageG &y, const Message &u, const Channel &channel,
                   int n, int i, int pref = 0) const = 0;

    virtual void
    updateUs(std::vector<std::vector<Symbol>> &us, int n, int pref, int i, const Symbol &symbol) const;

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