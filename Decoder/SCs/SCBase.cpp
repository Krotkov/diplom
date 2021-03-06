//
// Created by kranya on 30.04.2021.
//

#include <cmath>
#include <utils/utils.h>
#include <Code/PolarCode/PolarCode.h>
#include "SCBase.h"

std::vector<double> SCBase::calcZ(const Channel &channel, int iters) const {
    std::vector<double> ans(n_);
    Message coded;
    for (int i = 0; i < n_; i++) {
        coded.add(0);
    }

    for (int j = 0; j < iters; j++) {
        if (j % 1000 == 0) {
            std::cout << j << "\n";
        }
        auto message1 = channel.runMessage(coded);

        std::pair<Message, std::vector<double>> decoded;
        decoded.first.resize(n_);
        decoded.second.resize(n_);
        std::vector<bool> flip;
        flip.resize(n_, false);

        calculateL(message1, 0, 0, channel, flip, decoded, true);

        for (int i = 0; i < n_; i++) {
            if (decoded.second[i] < 0) {
                ans[i] += 1;
            }
        }
    }
    for (int j = 0; j < n_; j++) {
        ans[j] /= iters;
    }

    return ans;
}


Message SCBase::decode(const MessageG &message, const Channel &channel) const {
    std::pair<Message, std::vector<double>> decoded1;
    decoded1.first.resize(n_);
    decoded1.second.resize(n_);
    std::vector<bool> flip;
    flip.resize(n_, false);

    auto ans = code_.reverseEncode(calculateL(message, 0, 0, channel, flip, decoded1));

    Message ans1;
    for (int i = 0; i < ans.size(); i++) {
        if (!frozen_[i]) {
            ans1.add(ans[i]);
        }
    }
    return ans1;
}

Message SCBase::calculateL(const MessageG &message1, int n, int i, const Channel &channel, const std::vector<bool> &flip,
                           std::pair<Message, std::vector<double>> &decoderAnswer, bool calcZMode) const {
    MessageG message = message1;
    if (message.size() == n_) {
        for (int j = 0; j < message.size(); j++) {
            message[j] = channel.getLLR(message[j]);
        }
    }

    if (message.size() == 1) {
        Message ans;
        if (calcZMode) {
            ans.add(0);
            decoderAnswer.first[i] = 0;
        } else {
            if (frozen_[i] || message[0] > 0) {
                ans.add(0);
                decoderAnswer.first[i] = 0;
            } else {
                ans.add(1);
                decoderAnswer.first[i] = 1;
            }
        }

        //flip
        if (flip[i]) {
            ans.back() += 1;
            decoderAnswer.first[i] += 1;
        }
        decoderAnswer.second[i] = message[0];
        return ans;
    }

    if (specialNodes_[n][i] == REP) {
        double sum = 0;
        for (int j = 0; j < message.size(); j++) {
            sum += message[j];
        }
        int newI = i;
        for (int j = n+1; j < specialNodes_.size(); j++) {
            newI = newI * kernel_.getN() + kernel_.getN() - 1;
        }

        Symbol a;
        if ((sum >= 0 && !flip[newI]) || (sum < 0 && flip[newI])) {
            a = 0;
            decoderAnswer.first[newI] = 0;
        } else {
            a = 1;
            decoderAnswer.first[newI] = 1;
        }
        Message ans;
        for (int j = 0; j < message.size(); j++) {
            ans.add(a);
        }

        decoderAnswer.second[newI] = sum;

        return ans;
    }

    std::vector<MessageG> parts(message.size() / kernel_.size());

    for (int j = 0; j < parts.size(); j++) {
        parts[j].reserve(kernel_.size());
        for (int q = j * kernel_.size(); q < (j + 1) * kernel_.size(); q++) {
            parts[j].add(message[q]);
        }
    }

    MessageG curY;
    curY.resize(message.size() / kernel_.size());
    std::vector<Message> us(curY.size());
    for (int j = 0; j < kernel_.size(); j++) {
        Message curU;
        int newN = n+1;
        int newI = i * kernel_.getN() + j;

        if (specialNodes_[newN][newI] == RATE0) {
            for (int q = 0; q < curY.size(); q++) {
                curU.add(0);
            }
        } else {
            for (int q = 0; q < curY.size(); q++) {
                curY[q] = calculateLStep(parts[q], us[q], channel);
            }
            curU = calculateL(curY, n + 1, i * kernel_.getN() + j, channel, flip, decoderAnswer, calcZMode);
        }
        for (int q = 0; q < curU.size(); q++) {
            us[q].add(curU[q]);
        }
    }

    Message ans;
    ans.reserve(message.size());
    for (int j = 0; j < us.size(); j++) {
        us[j] = dot(Matrix(us[j]), kernel_).getRow(0);
        for (int q = 0; q < us[j].size(); q++) {
            ans.add(us[j][q]);
        }
    }
    return ans;
}

SCBase::SCBase(const PolarCode &code) {
    n_ = code.getN();
    frozen_ = code.getFrozen();
    kernel_ = code.getKernel();
    code_ = code;

    int ln = getLog(n_, kernel_.getN());
    specialNodes_.resize(ln + 1);
    int a = 1;
    for (int i = 0; i < specialNodes_.size(); i++) {
        specialNodes_[i].resize(a, NONE);
        a *= kernel_.getN();
    }
    recursiveSpecialNodesCalc(0, 0, 0, n_);
}

void SCBase::recursiveSpecialNodesCalc(int n, int i, int l, int r) {
    if (r - l == 1) {
        nodeList_.emplace_back(n, i);
        return;
    }

    bool flagRate0 = true, flagRep = true, flagSpc = true, flagRate1 = true;
    for (int j = l; j < r; j++) {
        if (!frozen_[j]) {
            flagRate0 = false;
        }
        if (j < r-1 && !frozen_[j]) {
            flagRep = false;
        }
        if (j == r-1 && frozen_[j]) {
            flagRep = false;
        }
        if (frozen_[j]) {
            flagRate1 = false;
        }
        if (j != 0 && frozen_[j]) {
            flagSpc = false;
        }
        if (j == 0 && !frozen_[j]) {
            flagSpc = false;
        }
    }
    if (flagRate0) {
        specialNodes_[n][i] = RATE0;
        return;
    }
    if (flagRep) {
        specialNodes_[n][i] = REP;
    }
    if (flagSpc) {
        specialNodes_[n][i] = SPC;
        nodeList_.emplace_back(n, i);
        return;
    }
    if (flagRate1) {
        specialNodes_[n][i] = RATE1;
    }

    for (int j = 0; j < kernel_.getN(); j++) {
        recursiveSpecialNodesCalc(n + 1, i * kernel_.getN() + j, l + (r - l) * j / kernel_.getN(),
                                  l + (r - l) * (j + 1) / kernel_.getN());
    }
}
