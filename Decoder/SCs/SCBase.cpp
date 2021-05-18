//
// Created by kranya on 30.04.2021.
//

#include <cmath>
#include <utils/utils.h>
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

        calculateL(message1, 0, channel, flip, decoded, true);

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

    calculateL(message, 0, channel, flip, decoded1);

    Message ans1;
    for (int i = 0; i < decoded1.first.size(); i++) {
        if (!frozen_[i]) {
            ans1.add(decoded1.first[i]);
        }
    }
    return ans1;
}

Message SCBase::calculateL(const MessageG &message1, int i, const Channel &channel, const std::vector<bool> &flip,
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

    std::vector<MessageG> parts(message.size() / kernel_.size());

    for (int j = 0; j < parts.size(); j++) {
        parts[j].reserve(kernel_.size());
        for (int q = j * kernel_.size(); q < (j + 1) * kernel_.size(); q++) {
            parts[j].add(message[q]);
        }
    }

    MessageG curY;
    Message curU;
    curY.resize(message.size() / kernel_.size());
    std::vector<Message> us(curY.size());
    for (int j = 0; j < kernel_.size(); j++) {
        for (int q = 0; q < curY.size(); q++) {
            curY[q] = calculateLStep(parts[q], us[q], channel);
        }
        curU = calculateL(curY, i * kernel_.getN() + j, channel, flip, decoderAnswer, calcZMode);
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
