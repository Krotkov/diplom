//
// Created by kranya on 06.05.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCFlip.h"
#include "SCFlipViterbi.h"
#include "SCFlipArikan.h"
#include <set>

SCFlip::SCFlip(const CrcPolarCode &code, double a, int iters) {
    code_ = code;
    if (code_.getKernel().getN() == 2) {
        sc_ = new SCFlipArikan(code);
    } else {
        sc_ = new SCFlipViterbi(code);
    }
    a_ = a;
    iters_ = iters;
    frozen_ = code.getFrozen();
}

SCFlip::~SCFlip() {
    delete sc_;
}

Message SCFlip::decode(const MessageG &message, const Channel &channel) const {
    int ln = getLog(code_.getN(), code_.getKernel().size());
    std::vector<std::vector<double>> l_;
    std::vector<std::vector<Symbol>> us;
    l_.resize(ln + 1);
    us.resize(ln + 1);

    for (int i = 0; i < ln + 1; i++) {
        l_[i].resize(code_.getN(), NAN);
        us[i].resize(code_.getN(), Symbol(-1));
    }

    std::vector<int> curE;
    auto firstTry = decodeStep(message, channel, l_, us, curE);

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
                flips.insert({-ma, flip});
            } else if (-flips.cbegin()->first > ma) {
                flips.erase(flips.cbegin());
                flips.insert({-ma, flip});
            }
        }
    }

    std::pair<Message, std::vector<double>> decodedRes;

    if (iters_ == 0) {
        return cutCrc(firstTry.first);
    }

    for (int i = 0; i < iters_; i++) {
        decodedRes = decodeStep(message, channel, l_, us, flips.begin()->second);

        if (code_.check(decodedRes.first)) {
            return cutCrc(decodedRes.first);
        }

        auto flip = flips.begin()->second;
        flip.push_back(-1);
        for (int j = flips.begin()->second.back() + 1; j < code_.getN(); j++) {
            if (!frozen_[j]) {
                flip.back() = j;
                double ma = calcMa(decodedRes.second, flip);
                if (-flips.cbegin()->first > ma) {
                    flips.erase(flips.cbegin());
                    flips.insert({-ma, flip});
                }
            }
        }
        flips.erase(flips.begin());
    }

    return cutCrc(decodedRes.first);
}

std::pair<Message, std::vector<double>>
SCFlip::decodeStep(const MessageG &message, const Channel &channel, std::vector<std::vector<double>> &l_,
                         std::vector<std::vector<Symbol>> &us,
                         const std::vector<int> &flip) const {
    int ln = getLog(code_.getN(), code_.getKernel().size());
    for (int i = 0; i <= ln; i++) {
        for (int j = 0; j < code_.getN(); j++) {
            l_[i][j] = NAN;
            us[i][j] = Symbol(-1);
        }
    }

    Message decoded;
    std::vector<double> ls;
    int ind = 0;
    for (int i = 0; i < message.size(); i++) {
        if (frozen_[i]) {
            decoded.add(0);
            ls.push_back(0);
        } else {
            double value = sc_->calculateL(l_, us, message, channel, ln, i);
            ls.push_back(value);
            if (value > 0) {
                decoded.add(0);
            } else {
                decoded.add(1);
            }

            //flip if need
            if (ind < flip.size() && i == flip[ind]) {
                decoded.back() += 1;
                ind++;
            }
        }
        if (i != code_.getN() - 1) {
            sc_->updateUs(us, ln, 0, i + 1, decoded.back());
        }
    }

    Message ans;
    for (int i = 0; i < decoded.size(); i++) {
        if (!frozen_[i]) {
            ans.add(decoded[i]);
        }
    }
    return {ans, ls};

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

