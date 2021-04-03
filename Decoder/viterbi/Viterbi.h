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
    explicit Viterbi(const Matrix& matrix);

    Message decode(const Message& message, const Channel& channel) const override;

private:
    void build_grid();

    std::vector<std::vector<Node>> grid_;

    Matrix matrix_;
};
