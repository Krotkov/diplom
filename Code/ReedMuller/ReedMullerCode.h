#pragma once

#include <Code/PolarCode/PolarCode.h>
#include <utils/utils.h>
#include <iostream>

class ReedMullerCode : public PolarCode {
public:
    ReedMullerCode(int n, int k, int d) : PolarCode(n, k, 0.5) {
        int m = getLog(n);
        int r = getLog(n / d);

//        for (int i = 0; i < g_.size(); i++) {
//            g_.getRow(i).print();
//        }

        for (auto && i : frozen_) {
            i = false;
        }
        for (int i = 0; i < g_.size(); i++) {
            if (g_.getRow(i).getWeight() < d) {
                frozen_[i] = true;
            }
        }

//        std::cout << "\n";
//        for (int i = 0; i < frozen_.size(); i++) {
//            std::cout << frozen_[i] << " ";
//        }
//        std::cout << "\n";
    };
};
