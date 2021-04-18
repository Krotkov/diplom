#pragma once

#include <vector>
#include <Code/Code.h>
#include "Matrix/Matrix.h"
#include "Message/Message.h"

class PolarCode : public Code {
public:
    PolarCode(int n, int k, double err);

    void constructCode(int n, int k, double err);

    Message encode(const Message& message) const override;

protected:

    double calculateZ(int n, int i, double err) const;
};