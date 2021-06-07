#pragma once

#include <Channel/Channel.h>
#include <Decoder/Decoder.h>
#include "Matrix/Matrix.h"

struct Node {
    int next_0 = -1;
    int next_1 = -1;
    Symbol s_0;
    Symbol s_1;
};

class Viterbi : public Decoder {
public:
    Viterbi() = default;

    explicit Viterbi(const Matrix &matrix);

    Viterbi(const Viterbi &viterbi);

    Viterbi &operator=(const Viterbi &other);

    Viterbi &operator=(Viterbi &&other) noexcept;

    Message decode(const MessageG &message, const Channel &channel) const override;

    double calcLLr(const MessageG &message, const Channel &channel) const;

private:
    std::vector<std::vector<std::pair<double, int>>> count_dp(const MessageG &message, const Channel &channel) const;

    void build_grid();

    std::vector<std::vector<Node>> grid_;

    Matrix matrix_;
};
