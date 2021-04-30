#pragma once

#include <Code/PolarCode/PolarCodeWithArikan.h>
#include <utils/utils.h>
#include <iostream>

class ReedMullerCode : public PolarCodeWithArikan {
public:
    ReedMullerCode(int n, int k, int d) : PolarCodeWithArikan(n, k, 0.5) {
        for (auto && i : frozen_) {
            i = false;
        }
        for (int i = 0; i < g_.size(); i++) {
            if (g_.getRow(i).getWeight() < d) {
                frozen_[i] = true;
            }
        }
    };
};
