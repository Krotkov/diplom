#pragma once

#include <vector>
#include <cassert>
#include "Matrix/Matrix.h"

class Message : public std::vector<Symbol> {
public:
    Message() : vector() {};

    void add(const Symbol &s);

    void print();

    void mulOnMatrix(const Matrix& matrix);
};
