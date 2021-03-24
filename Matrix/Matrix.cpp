//
// Created by kranya on 12.03.2021.
//

#include <iostream>
#include "Matrix.h"

Matrix &Matrix::operator=(const Matrix &other) {
    std::vector<std::vector<Symbol>>::operator=(other);

    this->n_ = other.n_;
    this->k_ = other.k_;

    return *this;
}

Matrix &Matrix::operator=(Matrix &&other) noexcept {
    std::vector<std::vector<Symbol>>::operator=(other);

    this->n_ = other.n_;
    this->k_ = other.k_;;

    return *this;
}

Matrix getIdMatrix(int n) {
    Matrix ans(n, n);

    for (int i = 0; i < n; i++) {
        ans[i][i] = 1;
    }

    return ans;
}

Matrix dot(const Matrix &a, const Matrix &b) {
    assert(a.k_ == b.n_);
    Matrix ans(a.n_, b.k_);
    for (int i = 0; i < a.n_; i++) {
        for (int j = 0; j < b.k_; j++) {
            for (int q = 0; q < a.k_; q++) {
                ans[i][j] += a[i][q] * b[q][j];
            }
        }
    }
    return ans;
}

Matrix kronMul(const Matrix &a, const Matrix &b) {
    Matrix ans(a.n_ * b.n_, a.k_ * b.k_);

    for (int i = 0; i < a.n_; i++) {
        for (int j = 0; j < a.k_; j++) {
            for (int q = 0; q < b.n_; q++) {
                for (int w = 0; w < b.k_; w++) {
                    ans[q * a.n_ + i][w * a.k_ + j] = a[i][j] * b[q][w];
                }
            }
        }
    }

    return ans;
}

Matrix kronPower(const Matrix &a, int n) {
    Matrix ans(1, 1);

    ans[0][0] = 1;

    for (int i = 0; i < n; i++) {
        ans = kronMul(a, ans);
    }

    return ans;
}

int Matrix::getN() const {
    return n_;
}

int Matrix::getK() const {
    return k_;
}

void Matrix::print() const {
    for (int i = 0; i < getN(); i++) {
        for (int j = 0; j < getK(); j++) {
            std::cout << (*this)[i][j].get();
        }
        std::cout << "\n";
    }
}

Matrix transpose(const Matrix &a) {
    Matrix ans(a.k_, a.n_);
    for (int i = 0; i < ans.n_; i++) {
        for (int j = 0; j < ans.k_; j++) {
            ans[i][j] = a[j][i];
        }
    }
    return ans;
}

Message Matrix::getRow(int ind) const {
    assert(ind < this->n_);
    Message ans;

    for (int i = 0; i < this->k_; i++) {
        ans.add((*this)[ind][i]);
    }

    return ans;
}

Message Matrix::getColumn(int ind) const {
    assert(ind < this->k_);
    Message ans;

    for (int i = 0; i < this->n_; i++) {
        ans.add((*this)[i][ind]);
    }

    return ans;
}
