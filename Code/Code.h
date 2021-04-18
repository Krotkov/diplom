#pragma once

#include <Message/Message.h>
#include <Matrix/Matrix.h>
#include <utils/utils.h>

class Code {
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

    virtual ~Code() = default;

protected:
    virtual Matrix calcR(const Matrix &a) const {
        Matrix ans(a.getN(), a.getK());

        for (int i = 0; i < a.getN(); i++) {
            int ind = 0;
            for (int j = 0; j < a.getK(); j += 2) {
                ans[i][ind] = a[i][j];
                ind++;
            }
            for (int j = 1; j < a.getK(); j += 2) {
                ans[i][ind] = a[i][j];
                ind++;
            }
        }
        return ans;
    }


    virtual Matrix calcBn(int n) const {
        assert(checkIfPowerOfTwo(n));

        Matrix idMatrix = getIdMatrix(2);
        Matrix b = idMatrix;

        for (int i = 4; i <= n; i *= 2) {
            b = calcR(kronMul(idMatrix, b));
        }

        return b;
    }

    int n_;
    int k_;
    Matrix g_;
    std::vector<bool> frozen_;
    Matrix kernel_;
};