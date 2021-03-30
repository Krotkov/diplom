#pragma once

#include "Matrix/Matrix.h"

struct Node {
    int next_0 = -1;
    int next_1 = -1;
    Symbol s_0;
    Symbol s_1;
};

class Viterbi {
public:
    explicit Viterbi(const Matrix& matrix, int n, int k, double noise);

    Message decode(const Message& message);

private:
    void build_grid();

    std::vector<std::vector<Node>> grid_;

    Matrix matrix_;
    int n_;
    int k_;
    double noise_;
};
