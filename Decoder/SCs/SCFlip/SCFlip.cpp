//
// Created by kranya on 06.05.2021.
//

#include <cmath>
#include <utils/utils.h>
#include "SCFlip.h"
#include "SCFlipViterbi.h"
#include <set>
#include <Decoder/SCs/SC/SC.h>

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

    auto comparator = [](const std::pair<double, std::vector<NodeFlip>> &a,
                         const std::pair<double, std::vector<NodeFlip>> &b) {
        return a.first < b.first;
    };
    std::set<std::pair<double, std::vector<NodeFlip>>, decltype(comparator)> flips;
    auto nodeList = sc_->getNodeList();
    auto specialNodes = sc_->getSpecialNodes();
    for (int i = 0; i < nodeList.size(); i++) {
        std::vector<NodeFlip> curFlips;
        int curN = nodeList[i].first, curI = nodeList[i].second;
        double ma;
        if (specialNodes[curN][curI] == LEAF || specialNodes[curN][curI] == REP) {
            curFlips.emplace_back(curN, curI);
            ma = calcMa(curFlips, nodes_l, channel);
            if (flips.size() < iters_) {
                flips.insert({ma, curFlips});
            } else if (flips.rbegin()->first > ma) {
                flips.erase(--flips.end());
                flips.insert({ma, curFlips});
            }
            curFlips.pop_back();
        } else if (specialNodes[curN][curI] == RATE1) {
            for (int j = 0; j < flips_[curN][curI].size(); j++) {
                curFlips.emplace_back(curN, curI, j);
                ma = calcMa(curFlips, nodes_l, channel);
                if (flips.size() < iters_) {
                    flips.insert({ma, curFlips});
                } else if (flips.rbegin()->first > ma) {
                    flips.erase(--flips.end());
                    flips.insert({ma, curFlips});
                }
                curFlips.pop_back();
            }
        } else if (specialNodes[curN][curI] == SPC) {
            for (int j = 0; j < flips_[curN][curI].size(); j++) {
                for (int q = j + 1; q < flips_[curN][curI].size(); q++) {
                    curFlips.emplace_back(curN, curI, j, q);
                    ma = calcMa(curFlips, nodes_l, channel);
                    if (flips.size() < iters_) {
                        flips.insert({ma, curFlips});
                    } else if (flips.rbegin()->first > ma) {
                        flips.erase(--flips.end());
                        flips.insert({ma, curFlips});
                    }
                    curFlips.pop_back();
                }
            }
        }
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
            if (j == ind) {
                if (specialNodes[nodeList[j].first][nodeList[j].second] == LEAF ||
                    specialNodes[nodeList[j].first][nodeList[j].second] == REP) {
                    continue;
                }
            }
            double ma;
            if (specialNodes[nodeList[j].first][nodeList[j].second] == LEAF ||
                specialNodes[nodeList[j].first][nodeList[j].second] == REP) {
                flip.emplace_back(nodeList[j].first, nodeList[j].second);
                ma = calcMa(flip, nodes_l, channel);
                if (flips.size() < iters_) {
                    flips.insert({ma, flip});
                } else if (flips.rbegin()->first > ma) {
                    flips.erase(--flips.end());
                    flips.insert({ma, flip});
                }
                flip.pop_back();
            } else if (specialNodes[nodeList[j].first][nodeList[j].second] == RATE1) {
                int minInd = 0;
                if (j == ind) {
                    minInd = flip.back().f1 + 1;
                }
                for (int q = minInd; q < flips_[nodeList[j].first][nodeList[j].second].size(); q++) {
                    flip.emplace_back(nodeList[j].first, nodeList[j].second, q);
                    ma = calcMa(flip, nodes_l, channel);
                    if (flips.size() < iters_) {
                        flips.insert({ma, flip});
                    } else if (flips.rbegin()->first > ma) {
                        flips.erase(--flips.end());
                        flips.insert({ma, flip});
                    }
                    flip.pop_back();
                }
            } else if (specialNodes[nodeList[j].first][nodeList[j].second] == SPC) {
                int minInd = 0;
                if (j == ind) {
                    minInd = flip.back().f2 + 1;
                }
                for (int q = minInd; q < flips_[nodeList[j].first][nodeList[j].second].size(); q++) {
                    for (int w = q + 1; w < flips_[nodeList[j].first][nodeList[j].second].size(); w++) {
                        flip.emplace_back(nodeList[j].first, nodeList[j].second, q, w);
                        ma = calcMa(flip, nodes_l, channel);
                        if (flips.size() < iters_) {
                            flips.insert({ma, flip});
                        } else if (flips.rbegin()->first > ma) {
                            flips.erase(--flips.end());
                            flips.insert({ma, flip});
                        }
                        flip.pop_back();
                    }
                }
            }
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

