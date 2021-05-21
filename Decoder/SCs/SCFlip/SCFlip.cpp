//
// Created by kranya on 06.05.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCFlip.h"
#include "SCFlipViterbi.h"
#include <set>
#include <Decoder/SCs/SC/SC.h>

SCFlip::SCFlip(const CrcPolarCode &code, double a, int iters) {
    code_ = code;
    if (code_.getKernel().getN() == 2) {
        sc_ = new SC(code.getPolarCode());
    } else {
        sc_ = new SCViterbi(code.getPolarCode());
    }
    a_ = a;
    iters_ = iters;
    frozen_ = code.getFrozen();
}

SCFlip::~SCFlip() {
    delete sc_;
}

Message SCFlip::decode(const MessageG &message, const Channel &channel) const {
   std::vector<int> curE;
    auto firstTry = decodeStep(message, channel, curE);

    if (code_.check(firstTry.first)) {
        return cutCrc(firstTry.first);
    }

    std::set<std::pair<double, std::vector<int>>> flips;
    for (int i = 0; i < code_.getN(); i++) {
        if (!frozen_[i]) {
            std::vector<int> flip;
            flip.push_back(i);
            double ma = calcMa(firstTry.second, flip);
            if (flips.size() < iters_) {
                flips.insert({ma, flip});
            } else if (flips.rbegin()->first > ma) {
                flips.erase(--flips.end());
                flips.insert({ma, flip});
            }
        }
    }

    std::pair<Message, std::vector<double>> decodedRes;

    for (int i = 0; i < iters_; i++) {
        decodedRes = decodeStep(message, channel, flips.begin()->second);

        if (code_.check(decodedRes.first)) {
            return cutCrc(decodedRes.first);
        }

        auto flip = flips.begin()->second;
        flip.push_back(-1);
        for (int j = flips.begin()->second.back() + 1; j < code_.getN(); j++) {
            if (!frozen_[j]) {
                flip.back() = j;
                double ma = calcMa(decodedRes.second, flip);
                if (flips.rbegin()->first > ma) {
                    flips.erase(--flips.end());
                    flips.insert({ma, flip});
                }
            }
        }
        flips.erase(flips.begin());
        if (flips.empty()) {
            break;
        }
    }

    return cutCrc(decodedRes.first);
}

std::pair<Message, std::vector<double>>
SCFlip::decodeStep(const MessageG &message, const Channel &channel, const std::vector<int> &flip) const {
    std::pair<Message, std::vector<double>> decoded1;
    decoded1.first.resize(code_.getN());
    decoded1.second.resize(code_.getN());
    std::vector<bool> flip2;
    flip2.resize(code_.getN(), false);
    for (auto i: flip) {
        flip2[i] = true;
    }

    sc_->calculateL(message, 0, 0, channel, flip2, decoded1);

    Message ans1;
    for (int i = 0; i < decoded1.first.size(); i++) {
        if (!frozen_[i]) {
            ans1.add(decoded1.first[i]);
        }
    }
    decoded1.first = ans1;
    return decoded1;
}

double SCFlip::calcMa(const std::vector<double> &l, const std::vector<int> &flip) const {
    double ans = 0;
    for (int i = 0; i < flip.size(); i++) {
        ans += std::abs(l[flip[i]]);
    }

    double ans2 = 0;
    for (int i = 0; i <= flip.back(); i++) {
        if (!frozen_[i]) {
            ans2 += std::log(1 + std::exp(-a_ * std::abs(l[i])));
        }
    }
    ans2 /= a_;
    return ans + ans2;
}

Message SCFlip::cutCrc(const Message &message) const {
    Message ans;
    for (int j = 0; j < code_.getK(); j++) {
        ans.add(message[j]);
    }
    return ans;
}

