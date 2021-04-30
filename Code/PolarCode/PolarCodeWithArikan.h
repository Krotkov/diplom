#pragma once

#include <vector>
#include <Code/PolarCode.h>
#include "Matrix/Matrix.h"
#include "Message/Message.h"

class PolarCodeWithArikan : public PolarCode {
public:
    PolarCodeWithArikan(int n, int k, double err);

    void constructCode(int n, int k, double err);

protected:

    double calculateZ(int n, int i, double err) const;
};