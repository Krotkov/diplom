//
// Created by kranya on 06.05.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCFlip.h"
#include <set>
#include <Decoder/SCs/SC/SC.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>

SCFlip::SCFlip(const CrcPolarCode &code, double a, int iters, bool useSpecialNodes) {
    code_ = code;
    if (code_.getKernel().getN() == 2) {
        sc_ = new SC(code.getPolarCode(), useSpecialNodes);
    } else {
        sc_ = new SCViterbi(code.getPolarCode(), useSpecialNodes);
    }
    a_ = a;
    iters_ = iters;
    frozen_ = code.getFrozen();
}

SCFlip::~SCFlip() {
    delete sc_;
}

Message SCFlip::decode(const MessageG &message, const Channel &channel) const {
    std::vector<NodeFlip> curE;

    auto nodes_l = sc_->getNodesLShape();
    auto flips_ = sc_->getFlipsShape();

    auto firstTry = decodeStep(message, channel, curE, nodes_l, flips_);

    if (code_.check(firstTry)) {
        return cutCrc(firstTry);
    }

    std::set<std::pair<double, std::vector<NodeFlip>>> flips;
    auto nodeList = sc_->getNodeList();
    std::vector<NodeFlip> curFlips;
    for (int i = 0; i < nodeList.size(); i++) {
        tryInsertNode(nodeList[i], flips, curFlips, nodes_l, channel);
    }

    Message decodedRes;

    for (int i = 0; i < iters_; i++) {
        decodedRes = decodeStep(message, channel, flips.begin()->second, nodes_l, flips_);

        if (code_.check(decodedRes)) {
            return cutCrc(decodedRes);
        }

        auto flip = flips.begin()->second;
        int lastN = -1, lastI = -1;
        if (!flip.empty()) {
            lastN = flip.back().n;
            lastI = flip.back().i;
        }
        int ind = 0;
        if (lastN != -1) {
            for (int j = 0; j < nodeList.size(); j++) {
                if (nodeList[j].first == lastN && nodeList[j].second == lastI) {
                    ind = j;
                    break;
                }
            }
        }

        for (int j = ind; j < nodeList.size(); j++) {
            tryInsertNode(nodeList[j], flips, flip, nodes_l, channel);
        }
        flips.erase(flips.begin());
        if (flips.empty()) {
            break;
        }
    }

    return cutCrc(decodedRes);
}

Message
SCFlip::decodeStep(const MessageG &message, const Channel &channel, const std::vector<NodeFlip> &flip,
                   std::vector<std::vector<MessageG>> &nodes_l_,
                   std::vector<std::vector<std::vector<bool>>> &flips_) const {
    for (int i = 0; i < flips_.size(); i++) {
        for (int j = 0; j < flips_[i].size(); j++) {
            for (int q = 0; q < flips_[i][j].size(); q++) {
                flips_[i][j][q] = false;
            }
        }
    }
    for (int i = 0; i < flip.size(); i++) {
        if (sc_->specialNodes_[flip[i].n][flip[i].i] == LEAF) {
            flips_[flip[i].n][flip[i].i][0] = true;
        } else if (sc_->specialNodes_[flip[i].n][flip[i].i] == REP) {
            for (int j = 0; j < flips_[flip[i].n][flip[i].i].size(); j++) {
                flips_[flip[i].n][flip[i].i][j] = true;
            }
        } else if (sc_->specialNodes_[flip[i].n][flip[i].i] == RATE1) {
            flips_[flip[i].n][flip[i].i][flip[i].f1] = true;
        } else if (sc_->specialNodes_[flip[i].n][flip[i].i] == SPC) {
            flips_[flip[i].n][flip[i].i][flip[i].f1] = true;
            flips_[flip[i].n][flip[i].i][flip[i].f2] = true;
        }
    }

    auto ans = code_.getPolarCode().reverseEncode(sc_->calculateL(message, 0, 0, channel, nodes_l_, flips_));

    Message ans1;
    for (int i = 0; i < ans.size(); i++) {
        if (!frozen_[i]) {
            ans1.add(ans[i]);
        }
    }
    return ans1;
}

double SCFlip::calcMa(const std::vector<NodeFlip> &flip, std::vector<std::vector<MessageG>> &nodes_l_,
                      const Channel &channel) const {
    double ma2 = 0;
    int ind = 0;
    for (int i = 0; i < sc_->nodeList_.size(); i++) {
        int curN = sc_->nodeList_[i].first, curI = sc_->nodeList_[i].second;

        if (curN == flip.back().n && curI == flip.back().i) {
            break;
        }

        if (sc_->specialNodes_[curN][curI] == LEAF) {
            if (ind < flip.size() - 1 && flip[ind].n == curN && flip[ind].i == curI) {
                ma2 += std::abs(nodes_l_[curN][curI][0]);
                ind++;
            }
            ma2 += calcFa(std::abs(nodes_l_[curN][curI][0]));
        } else if (sc_->specialNodes_[curN][curI] == REP) {
            double w = 0;
            if (ind < flip.size() - 1 && flip[ind].n == curN && flip[ind].i == curI) {
                for (int j = 0; j < nodes_l_[curN][curI].size(); j++) {
                    w += nodes_l_[curN][curI][j];
                }
                ma2 += std::abs(w);
                ind++;
            }
            double q = 0;
            for (int j = 0; j < nodes_l_[curN][curI].size(); j++) {
                q += nodes_l_[curN][curI][j];
            }
            ma2 += calcFa(std::abs(q));
        } else if (sc_->specialNodes_[curN][curI] == RATE1) {
            while (ind < flip.size() - 1 && flip[ind].n == curN && flip[ind].i == curI) {
                ma2 += std::abs(nodes_l_[curN][curI][flip[ind].f1]);
                ind++;
            }
            for (int j = 0; j < nodes_l_[curN][curI].size(); j++) {
                ma2 += calcFa(std::abs(nodes_l_[curN][curI][j]));
            }
        } else if (sc_->specialNodes_[curN][curI] == SPC) {
            const MessageG &curMessage = nodes_l_[curN][curI];
            int min_ind = 0;
            for (int j = 0; j < curMessage.size(); j++) {
                if (std::abs(curMessage[j]) < std::abs(curMessage[min_ind])) {
                    min_ind = j;
                }
            }
            int parity = 0;
            for (int q = 0; q < nodes_l_[curN][curI].size(); q++) {
                if (nodes_l_[curN][curI][q] < 0) {
                    parity += 1;
                }
            }
            parity %= 2;
            while (ind < (int) flip.size() - 1 && flip[ind].n == curN && flip[ind].i == curI) {
                ma2 += std::abs(curMessage[flip[ind].f1]) - parity * std::abs(curMessage[min_ind]);
                ma2 += std::abs(curMessage[flip[ind].f2]) - parity * std::abs(curMessage[min_ind]);
                ind++;
            }
            for (int j = 0; j < nodes_l_[curN][curI].size(); j++) {
                if (j != min_ind) {
                    ma2 += calcFa(std::abs(curMessage[j]) + (1 - 2 * parity) * std::abs(curMessage[min_ind]));
                }
            }
        } else {
            throw std::runtime_error("bad special node");
        }
        if (curN == flip.back().n && curI == flip.back().i) {
            break;
        }
    }

    if (sc_->specialNodes_[flip.back().n][flip.back().i] == LEAF) {
        ma2 += std::abs(nodes_l_[flip.back().n][flip.back().i][0]);
    } else if (sc_->specialNodes_[flip.back().n][flip.back().i] == REP) {
        double q = 0;
        for (int j = 0; j < nodes_l_[flip.back().n][flip.back().i].size(); j++) {
            q += nodes_l_[flip.back().n][flip.back().i][j];
        }
        ma2 += std::abs(q);
    } else if (sc_->specialNodes_[flip.back().n][flip.back().i] == RATE1) {
        while (ind < flip.size()) {
            ma2 += std::abs(nodes_l_[flip[ind].n][flip[ind].i][flip[ind].f1]);
            ind++;
        }
    } else if (sc_->specialNodes_[flip.back().n][flip.back().i] == SPC) {
        const MessageG &curMessage = nodes_l_[flip.back().n][flip.back().i];
        int min_ind = 0;
        for (int j = 0; j < curMessage.size(); j++) {
            if (std::abs(curMessage[j]) < std::abs(curMessage[min_ind])) {
                min_ind = j;
            }
        }
        int parity = 0;
        for (int q = 0; q < nodes_l_[flip.back().n][flip.back().i].size(); q++) {
            if (nodes_l_[flip.back().n][flip.back().i][q] < 0) {
                parity += 1;
            }
        }
        parity %= 2;
        while (ind < flip.size()) {
            ma2 += std::abs(curMessage[flip[ind].f1]) - parity * std::abs(curMessage[min_ind]);
            ma2 += std::abs(curMessage[flip[ind].f2]) - parity * std::abs(curMessage[min_ind]);
            ind++;
        }
    }

    return ma2;
}

Message SCFlip::cutCrc(const Message &message) const {
    Message ans;
    for (int j = 0; j < code_.getK(); j++) {
        ans.add(message[j]);
    }
    return ans;
}

double SCFlip::calcFa(double val) const {
    return std::log(1 + std::exp(-a_ * val)) / a_;
}

void SCFlip::tryInsertNode(const std::pair<int, int> &node, std::set<std::pair<double, std::vector<NodeFlip>>> &flips,
                           std::vector<NodeFlip> &flip, std::vector<std::vector<MessageG>> &nodes_l_,
                           const Channel &channel) const {
    auto specialNodes = sc_->getSpecialNodes();
    if ((specialNodes[node.first][node.second] == LEAF ||
         specialNodes[node.first][node.second] == REP) && (flip.empty() ||
        (flip.back().n != node.first || flip.back().i != node.second))) {
        flip.emplace_back(node.first, node.second);
        tryInsertFlip(flips, flip, nodes_l_, channel);
        flip.pop_back();
    } else if (specialNodes[node.first][node.second] == RATE1) {
        int minInd = 0;
        if (!flip.empty() && flip.back().n == node.first && flip.back().i == node.second) {
            minInd = flip.back().f1 + 1;
        }
        for (int q = minInd; q < nodes_l_[node.first][node.second].size(); q++) {
            flip.emplace_back(node.first, node.second, q);
            tryInsertFlip(flips, flip, nodes_l_, channel);
            flip.pop_back();
        }
    } else if (specialNodes[node.first][node.second] == SPC) {
        int minInd = 0;
        if (!flip.empty() && flip.back().n == node.first && flip.back().i == node.second) {
            minInd = flip.back().f2 + 1;
        }
        for (int q = minInd; q < nodes_l_[node.first][node.second].size(); q++) {
            for (int w = q + 1; w < nodes_l_[node.first][node.second].size(); w++) {
                flip.emplace_back(node.first, node.second, q, w);
                tryInsertFlip(flips, flip, nodes_l_, channel);
                flip.pop_back();
            }
        }
    }
}

void SCFlip::tryInsertFlip(std::set<std::pair<double, std::vector<NodeFlip>>> &flips, const std::vector<NodeFlip> &flip,
                           std::vector<std::vector<MessageG>> &nodes_l_, const Channel &channel) const {
    double ma = calcMa(flip, nodes_l_, channel);
    if (flips.size() < iters_) {
        flips.insert({ma, flip});
    } else if (flips.rbegin()->first > ma) {
        flips.erase(--flips.end());
        flips.insert({ma, flip});
    }
}
