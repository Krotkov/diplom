//
// Created by kranya on 23.03.2021.
//

#include <iostream>
#include "Viterbi.h"

int getFirst(const std::vector<Symbol> &x) {
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == Symbol(1)) {
            return i;
        }
    }
    return -1;
}

int getLast(const std::vector<Symbol> &x) {
    for (int i = int(x.size()) - 1; i >= 0; i--) {
        if (x[i] == Symbol(1)) {
            return i;
        }
    }
    return -1;
}

void toSpanForm(Matrix &matrix) {
    auto spanComp = [](const std::vector<Symbol> &a, const std::vector<Symbol> &b) {
        return getFirst(a) < getFirst(b);
    };

    std::sort(matrix.begin(), matrix.end(), spanComp);

    //make different beginnings
    for (int q = 0; q < matrix.getK(); q++) {
        int cur_ind = 0;
        for (int i = 1; i < matrix.getN(); i++) {
            if (getFirst(matrix[i]) == getFirst(matrix[cur_ind])) {
                for (int j = 0; j < matrix.getK(); j++) {
                    matrix[i][j] += matrix[i - 1][j];
                }
            } else {
                cur_ind = i;
            }
        }
        std::sort(matrix.begin(), matrix.end(), spanComp);
    }

    //make different endings
    for (int w = 0; w < matrix.getK(); w++) {
        std::vector<std::vector<int>> ends;
        ends.resize(matrix.getK());
        for (int i = 0; i < matrix.getN(); i++) {
            ends[getLast(matrix[i])].push_back(i);
        }
        for (auto &end : ends) {
            if (end.size() >= 2) {
                for (int j = 0; j + 1 < end.size(); j++) {
                    for (int q = 0; q < matrix.getK(); q++) {
                        matrix[end[j]][q] += matrix[end.back()][q];
                    }
                }
            }
        }
    }
}


Viterbi::Viterbi(const Matrix &matrix) {
    matrix_ = matrix;
    matrix_.print();
    std::cout << "\n";
    toSpanForm(matrix_);
    matrix_.print();
}

