//
// Created by kranya on 19.04.2021.
//

#include <set>
#include "PolarCodeWithLargeKernel.h"

PolarCodeWithLargeKernel::PolarCodeWithLargeKernel(int n, int k, double err, const Matrix& kernel) {
    constructCode(n, k, err, kernel);
}

void PolarCodeWithLargeKernel::constructCode(int n, int k, double err, const Matrix& kernel) {
    n_ = n;
    k_ = k;
    kernel_ = kernel;

    std::vector<Message> words;

    a_.resize(kernel_.getN() + 1);
    for (int i = 0; i <= kernel_.getN(); i++) {
        a_[i].resize(kernel_.getN() + 1, 0);
    }
    d_.resize(kernel_.getN() + 1, int(1e9));
    for (int i = kernel_.getN() - 1; i >= 0; i--) {
        std::vector<Message> new_words;
        auto row = kernel_.getRow(i);
        new_words.push_back(row);
        for (const Message& word: words) {
            auto new_word = word + row;
            new_words.push_back(new_word);
        }
        d_[kernel_.getN() - i] = d_[kernel_.getN() - i - 1];
        for (const Message& word: new_words) {
            a_[kernel_.getN() - i][word.getWeight()]++;
            d_[kernel_.getN() - i] = std::min(d_[kernel_.getN() - i], word.getWeight());
            words.push_back(word);
        }
    }
    d_[0] = 0;

    std::vector<int> id(n_);
    for (int i = 0; i < n_; i++) {
        id[i] = i;
    }
    calcRn(id, 0, n_);

    std::vector<std::pair<double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {calcZ(getLog(n, kernel_.getN()), i, err, kernel_.getN()), id[i]};
    }

    std::sort(zz.begin(), zz.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });

    for (int i = 0; i < n_; i++) {
        std::cout << id[i] << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < n; i++) {
        std::cout << zz[i].first << " ";
    }
    std::cout << "\n";

    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n_, false);
    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }

    auto b = calcBn(n, kernel_.size());
    g_ = dot(calcBn(n, kernel_.size()), kronPower(kernel_, getLog(n, kernel_.size())));
}

double PolarCodeWithLargeKernel::calcZ(int n, int i, double err, int kernelSize) const {
    if (n == 0) {
        return err;
    } else {
        auto w = pow(kernelSize, n-1);
        int t = (i / pow(kernelSize, n)) * pow(kernelSize, n) + i % w;
        double ans = calcQ(calcZ(n-1, i % w, err, kernelSize), (i-t)/w);
        return ans;
    }
}

double PolarCodeWithLargeKernel::calcQ(double err, int nu) const {
    long double ans = 0;
    for (int e = d_[kernel_.getN() - nu]; e <= kernel_.getN(); e++) {
        int d = d_[kernel_.getN() - nu];
        long long b = 0;
        if (e < (3 * d + 1) / 2) {
            for (int i = d; i <= e; i++) {
                b += a_[kernel_.getN() - nu][i] * CnK(kernel_.getN() - i, e - i);
            }
        } else {
            for (int i = 1; i <= e; i++) {
                b += a_[kernel_.getN() - nu][i] * CnK(kernel_.getN() - i, e - i);
            }
            b = std::min(b, CnK(kernel_.getN(), e));
        }
        long double bb = b;
        for (int i = 0; i < e; i++) {
            bb *= err;
        }
        for (int i = 0; i < kernel_.getN() - e; i++) {
            bb *= (1 - err);
        }
        ans += bb;
    }
    return ans;
}

void PolarCodeWithLargeKernel::calcRn(std::vector<int> &x, int begin, int end) const {
    if (end - begin == 1) {
        return;
    }
    std::vector<int> new_x(end-begin);
    int ind = 0;
    for (int i = 0; i < kernel_.size(); i++) {
        for (int j = begin + i; j < end; j += kernel_.size()) {
            new_x[ind] = x[j];
            ind++;
        }
    }
    for (int i = begin; i < end; i++) {
        x[i] = new_x[i-begin];
    }
    for (int i = 0; i < kernel_.size(); i++) {
        calcRn(x, begin + i * ((end - begin) / kernel_.size()), begin + (i+1) * ((end - begin) / kernel_.size()));
    }
}
