#pragma once

#include <Decoder/Decoder.h>
#include <Matrix/Matrix.h>
#include <Code/PolarCode/PolarCode.h>
#include <map>

enum Special {
    NONE, RATE0, REP, SPC, RATE1, LEAF
};

struct NodeFlip {
    NodeFlip(int n_, int i_, int f1_ = -1, int f2_ = -1) : n(n_), i(i_), f1(f1_), f2(f2_) {};
    int n, i;
    int f1, f2;
};


class SCBase : public Decoder {
public:
    SCBase(const PolarCode &code);

    virtual Message
    calculateL(const MessageG &message, int n, int i, const Channel &channel,
               std::vector<std::vector<MessageG>> &nodes_l_,
               const std::vector<std::vector<std::vector<bool>>> &flips_,
               bool calcZMode = false) const;

    virtual double
    calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const = 0;

public:
    void recursiveSpecialNodesCalc(int n, int i, int l, int r);

    [[nodiscard]] virtual std::vector<double> calcZ(const Channel &channel, int iters = 100000) const;

    Message decode(const MessageG &message, const Channel &channel) const override;

    std::vector<std::pair<int, int>> getNodeList() const;

    std::vector<std::vector<Special>> getSpecialNodes() const {
        return specialNodes_;
    }

    std::vector<std::vector<MessageG>> getNodesLShape() const;

    std::vector<std::vector<std::vector<bool>>> getFlipsShape() const;

    std::vector<std::vector<Special>> specialNodes_;
    std::vector<std::pair<int, int>> nodeList_;

protected:
    int n_;
    std::vector<bool> frozen_;
//    std::vector<std::pair<int, int>> nodeIndex_;
    Matrix kernel_;
    PolarCode code_;
};