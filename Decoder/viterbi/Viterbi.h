#pragma once

#include "Matrix/Matrix.h"

class Viterbi {
public:
    explicit Viterbi(const Matrix& matrix);

//    Message decode(const Message& message);

private:

    Matrix matrix_;
};
