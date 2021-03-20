//
// Created by kranya on 13.03.2021.
//

#include <cmath>
#include <iostream>
#include "PolarCode.h"
#include "../utils/utils.h"
#include "../Channel/Channel.h"

void PolarCode::constructCode(int n, int k, double err, double noise) {
    n_ = n;
    k_ = k;
    err_ = err;
    noise_ = noise;

    std::vector<std::pair<double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {calculateZ(n, i + 1, err), i};
    }
    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n_, false);
    std::cout << "frozen: ";
    for (int i = 0; i < n-k; i++) {
        std::cout << i << " ";
        frozen_[zz[i].second] = true;
    }
    std::cout << "\n";

    Matrix f = Matrix(2, 2);
    f[0][0] = 1;
    f[0][1] = 0;
    f[1][0] = 1;
    f[1][1] = 1;

    g_ = dot(calcBn(n), kronPower(f, getLog(n)));
}

double PolarCode::calculateZ(int n, int i, double err) const {
    if (n == 1 && i == 1) {
        return err;
    }
    if (i % 2 == 0) {
        auto z = calculateZ(n / 2, i / 2, err);
        return z * z;
    } else {
        auto z = calculateZ(n / 2, (i + 1) / 2, err);
        return 2 * z - z * z;
    }
}

Matrix PolarCode::calcR(const Matrix &a) const {
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

Matrix PolarCode::calcBn(int n) const {
    assert(checkIfPowerOfTwo(n));

    Matrix idMatrix = getIdMatrix(2);
    Matrix b = idMatrix;

    for (int i = 4; i <= n; i *= 2) {
        b = calcR(kronMul(idMatrix, b));
    }

    return b;
}

PolarCode::PolarCode(int n, int k, double err, double noise) {
    constructCode(n, k, err, noise);
    int logN = getLog(n);
    l_.resize(logN+1);
    for (int i = 0; i < logN+1; i++) {
        l_[i].resize(n);
    }
}

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

    Message encoded;
    for (int j = 0; j < g_.getK(); j++) {
        encoded.add(0);
        for (int q = 0; q < expanded.size(); q++) {
            encoded[j] += expanded[q] * g_[q][j];
        }
    }

    return encoded;
}

Message PolarCode::decode(const Message &message) {
    int logN = getLog(n_);
    for (int i = 0; i < logN+1; i++) {
        for (int j = 0; j < n_; j++) {
            l_[i][j] = NAN;
        }
    }
    Message decoded;
    for (int i = 0; i < message.size(); i++) {
        double value = calculateL(message, decoded, logN, i);
        if (frozen_[i]) {
            decoded.add(0);
        } else {
            if (value > 0) {
                decoded.add(0);
            } else {
                decoded.add(1);
            }
        }
    }

    for (int i = 0; i < logN+1; i++) {
        for (int j = 0; j < n_; j++) {
            std::cout << l_[i][j] << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < decoded.size(); i++) {
        std::cout << decoded[i].get() << " ";
    }
    std::cout << "\n";

    Message ans;
    for (int i = 0; i < decoded.size(); i++) {
        if (!frozen_[i]) {
            ans.add(decoded[i]);
        }
    }

    return ans;
}

double PolarCode::calculateL(const Message &y, const Message &u, int n, int i, int pref) {
    if (n == 0) {
        l_[n][pref + i] = 2 * (y[0]).get() / Channel::sigma(n_, k_, noise_);
        std::cout << "sigma: " << Channel::sigma(n_, k_, noise_) << "\n";
        return l_[n][pref + i];
    }
    if (!std::isnan(l_[n][pref + i])) {
        return l_[n][i];
    }
    if (i % 2 == 0) {
        int next_pref = pref + (1 << (n - 1));
        double value;
        if (std::isnan(l_[n-1][next_pref + i / 2])) {
            Message new_y;
            for (int j = 0; j < y.size(); j+=2) {
                new_y.add(y[j]);
            }
            Message new_u;
            for (int j = 0; j < u.size(); j+=2) {
                new_u.add(u[j] + u[j+1]);
            }
            value = calculateL(new_y, new_u, n - 1, i / 2, next_pref);
        } else {
            value = l_[n-1][next_pref + i / 2];
        }
        std::cout << "tanh: " << value / 2 << " " << std::tanh(value / 2) << " " << 2 * std::atanh(std::tanh(value / 2) * std::tanh(value / 2)) << "\n";
        l_[n][pref + i] = 2 * std::atanh(std::tanh(value / 2) * std::tanh(value / 2));
        return l_[n][pref + i];
    } else {
        int next_pref_1 = pref;
        int next_pref_2 = pref + (1 << (n-1));
        double value1, value2;
        if (std::isnan(l_[n-1][next_pref_1 + (i - 1) / 2])) {
            Message new_y;
            for (int j = 1; j < y.size(); j += 2) {
                new_y.add(y[j]);
            }
            Message new_u;
            for (int j = 1; j < u.size() - 1; j += 2) {
                new_u.add(u[j]);
            }
            value1 = calculateL(new_y, new_u, n - 1, (i - 1) / 2, next_pref_1);
        } else {
            value1 = l_[n-1][next_pref_1 + (i - 1) / 2];
        }
        if (std::isnan(l_[n-1][next_pref_2 + (i - 1) / 2])) {
            Message new_y;
            for (int j = 0; j < y.size(); j += 2) {
                new_y.add(y[j]);
            }
            Message new_u;
            for (int j = 0; j < u.size() - 1; j += 2) {
                new_u.add(u[j] + u[j + 1]);
            }
            value2 = calculateL(new_y, new_u, n - 1, (i - 1) / 2, next_pref_2);
        } else {
            value2 = l_[n-1][next_pref_2 + (i - 1) / 2];
        }
        std::cout << "l: " << value1 << " " << value2 << " " << u[i-1].get() << "\n";
        l_[n][pref + i] = value1 + ((u[i - 1].get() == 0) ? value2 : -value2);
        std::cout << l_[n][pref + i] << "\n";
        return l_[n][pref + i];
    }
    return 0;
}
