#pragma once

#include <Message/Message.h>
#include <Matrix/Matrix.h>
#include <utils/utils.h>
#include <iostream>

class PolarCode {
public:
    virtual Message encode(const Message& message) const {
        assert(message.size() == k_);

        Message expanded;

        int ind = 0;
        for (int i = 0; i < n_; i++) {
            if (frozen_[i]) {
                expanded.add(0);
            } else {
                expanded.add(message[ind]);
                ind++;
            }
        }

        return dot(Matrix(expanded), g_).getRow(0);
    }

    virtual int getN() const {
        return n_;
    }
    virtual int getK() const {
        return k_;
    }

    virtual Matrix getG() const {
        Matrix ans(k_, n_);
        int ind = 0;
        for (int i = 0; i < n_; i++) {
            if (!frozen_[i]) {
                ans[ind] = g_[i];
                ind++;
            }
        }
        return ans;
    }

    virtual Matrix getFullG() const {
        return g_;
    }

    virtual std::vector<bool> getFrozen() const {
        return frozen_;
    }

    virtual  Matrix getKernel() const {
        return kernel_;
    }

    virtual ~PolarCode() = default;

protected:
    virtual Matrix calcR(const Matrix &a, int n) const {
        Matrix ans(a.getN(), a.getK());

        for (int i = 0; i < a.getN(); i++) {
            int ind = 0;
            for (int j = 0; j < n; j++) {
                for (int q = j; q < a.getK(); q += n) {
                    ans[i][ind] = a[i][q];
                    ind++;
                }
            }
        }
        return ans;
    }


    virtual Matrix calcBn(int n, int base) const {
        Matrix idMatrix = getIdMatrix(base);
        Matrix b = idMatrix;
        for (int i = base*base; i <= n; i *= base) {
            b = calcR(kronMul(idMatrix, b), base);
        }
        return b;
    }

    int n_;
    int k_;
    Matrix g_;
    std::vector<bool> frozen_;
    Matrix kernel_;
};