#pragma once

#include <vector>
#include <cassert>
#include "../Message/Message.h"


class Matrix : public std::vector<std::vector<Symbol> > {
public:

    Matrix() : n_(0), k_(0) {};

    Matrix(int n, int k) : n_(n), k_(k) {
        this->resize(n);
        for (int i = 0; i < n; i++) {
            (*this)[i].resize(k);
        }
    };

    Matrix(const Matrix &other) : vector(other) {
        this->n_ = other.n_;
        this->k_ = other.k_;
    };

    Matrix &operator=(const Matrix &other);

    Matrix &operator=(Matrix &&other) noexcept;

    friend Matrix dot(const Matrix &a, const Matrix &b);

    friend Matrix getIdMatrix(int n);

    friend Matrix kronMul(const Matrix &a, const Matrix &b);

    friend Matrix kronPower(const Matrix &a, int n);

    int getN() const;

    int getK() const;

private:

    int n_;
    int k_;
};

Matrix dot(const Matrix &a, const Matrix &b);

Matrix getIdMatrix(int n);

Matrix kronMul(const Matrix &a, const Matrix &b);

Matrix kronPower(const Matrix &a, int n);
