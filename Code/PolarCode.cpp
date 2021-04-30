#include <Code/PolarCode.h>

Message PolarCode::encode(const Message &message) const {
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

int PolarCode::getN() const {
    return n_;
}

int PolarCode::getK() const {
    return k_;
}

Matrix PolarCode::getG() const {
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

Matrix PolarCode::getFullG() const {
    return g_;
}

std::vector<bool> PolarCode::getFrozen() const {
    return frozen_;
}

Matrix PolarCode::getKernel() const {
    return kernel_;
}

Matrix PolarCode::calcR(const Matrix &a, int n) const {
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

Matrix PolarCode::calcBn(int n, int base) const {
    Matrix idMatrix = getIdMatrix(base);
    Matrix b = idMatrix;
    for (int i = base*base; i <= n; i *= base) {
        b = calcR(kronMul(idMatrix, b), base);
    }
    return b;
}
