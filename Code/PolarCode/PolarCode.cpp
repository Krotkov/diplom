#include <Code/PolarCode/PolarCode.h>
#include <utils/utils.h>
#include <Channel/Gaus/GausChannel.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>

Message PolarCode::encode(const Message &message) const {
    assert(message.size() == k_);

    Message expanded;

    int ind = 0;
    for (int i = 0; i < n_; i++) {
        if (frozen_[i]) {
            Symbol sum;
            if (dynamicFrozen_.contains(i)) {
                for (int j : dynamicFrozen_.at(i)) {
                    sum += expanded[j];
                }
            }
            expanded.add(sum);
        } else {
            expanded.add(message[ind]);
            ind++;
        }
    }

    recursiveEncode(expanded, 0, n_);
    return expanded;
}

void PolarCode::recursiveEncode(Message &message, int begin, int end) const {
    for (int i = begin; i < end; i += kernel_.size()) {
        Message cur;
        for (int q = i; q < i + kernel_.getN(); q++) {
            cur.add(message[q]);
        }
        cur = dot(Matrix(cur), kernel_).getRow(0);
        for (int q = i; q < i + kernel_.getN(); q++) {
            message[q] = cur[q - i];
        }
    }
    Message newMessage;
    newMessage.resize(end - begin);

    int ind = 0;
    for (int i = 0; i < kernel_.getN(); i++) {
        for (int j = begin + i; j < end; j += kernel_.getN()) {
            newMessage[ind] = message[j];
            ind++;
        }
    }
    for (int i = begin; i < end; i++) {
        message[i] = newMessage[i - begin];
    }
    if (end - begin > kernel_.getN()) {
        int size = (end - begin) / kernel_.getN();
        for (int i = 0; i < kernel_.getN(); i++) {
            recursiveEncode(message, begin + (i * size), begin + (i + 1) * size);
        }
    }
}

Message PolarCode::reverseEncode(const Message &message) const {
    Message ans = message;
    for (int a = kernel_.getN(); a <= n_; a *= kernel_.getN()) {
        for (int i = 0; i < n_; i += a) {
            reverseEncode(ans, i, i + a);
        }
    }
    return ans;
}

void PolarCode::reverseEncode(Message &message, int begin, int end) const {
    Message newMessage;
    newMessage.resize(end - begin);
    int ind = 0;
    for (int i = 0; i < (end - begin) / kernel_.getN(); i++) {
        for (int j = begin + i; j < end; j += (end - begin) / kernel_.getN()) {
            newMessage[ind] = message[j];
            ind++;
        }
    }
    for (int i = begin; i < end; i++) {
        message[i] = newMessage[i - begin];
    }

    for (int i = begin; i < end; i += kernel_.getN()) {
        Message cur;
        for (int q = i; q < i + kernel_.getN(); q++) {
            cur.add(message[q]);
        }
        cur = dot(Matrix(cur), rKernel_).getRow(0);
        for (int q = i; q < i + kernel_.getN(); q++) {
            message[q] = cur[q - i];
        }
    }
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
    for (int i = base * base; i <= n; i *= base) {
        b = calcR(kronMul(idMatrix, b), base);
    }
    return b;
}

void PolarCode::constructCode(int n, int k) {
    n_ = n;
    k_ = k;

    auto b = calcBn(n, kernel_.getN());
    g_ = dot(calcBn(n, kernel_.getN()), kronPower(kernel_, getLog(n, kernel_.getN())));
}

PolarCode::PolarCode(int n, int k, double err) {
    kernel_ = Matrix(2, 2);
    kernel_[0][0] = 1;
    kernel_[0][1] = 0;
    kernel_[1][0] = 1;
    kernel_[1][1] = 1;

    rKernel_ = kernel_;

    constructCode(n, k);

    std::vector<std::pair<double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {calculateZ(n, i + 1, err), i};
    }
    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    frozen_.resize(n, false);
    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }
}

PolarCode::PolarCode(int n, int k, const Matrix &kernel, const Matrix &rKernel) {
    kernel_ = kernel;
    rKernel_ = rKernel;

    constructCode(n, k);

    frozen_.resize(n, false);
    GausChannel channel(n, n, -3);
    SCViterbi viterbi(*this);

    auto z = viterbi.calcZ(channel, 10000);

    std::vector<std::pair<long double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {z[i], i};
    }

    std::sort(zz.begin(), zz.end());
    std::reverse(zz.begin(), zz.end());

    for (int i = 0; i < n - k; i++) {
        frozen_[zz[i].second] = true;
    }
}

PolarCode::PolarCode(int n, int k, const Matrix &kernel, const std::map<int, std::vector<int>> &dynamicFrozen) {
    kernel_ = kernel;

    constructCode(n, k);

    frozen_.resize(n, false);
    for (auto &a: dynamicFrozen) {
        frozen_[a.first] = true;
    }
    dynamicFrozen_ = dynamicFrozen;
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

std::map<int, std::vector<int>> PolarCode::getDynamicFrozen() const {
    return dynamicFrozen_;
}

