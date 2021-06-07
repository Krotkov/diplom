//
// Created by kranya on 06.05.2021.
//

#pragma once


#include <Decoder/Decoder.h>
#include <Decoder/SCs/SCBase.h>
#include <Code/CRC/CrcPolarCode.h>
#include <set>

class SCFlip : public Decoder {
public:
    SCFlip(const CrcPolarCode &code, double a = 0.5, int iters = 10, bool useSpecialNodes = true);

    ~SCFlip();

    Message decode(const MessageG &message, const Channel &channel) const override;

    Message
    decodeStep(const MessageG &message, const Channel &channel, const std::vector<NodeFlip> &flip,
               std::vector<std::vector<MessageG>> &nodes_l_,
               std::vector<std::vector<std::vector<bool>>> &flips_) const;

    void tryInsertNode(const std::pair<int, int> &node,
                       std::set<std::pair<double, std::vector<NodeFlip>>> &flips,
                       std::vector<NodeFlip> &flip,
                       std::vector<std::vector<MessageG>> &nodes_l_,
                       const Channel &channel) const;

    void tryInsertFlip(std::set<std::pair<double, std::vector<NodeFlip>>> &flips,
                       const std::vector<NodeFlip> &flip,
                       std::vector<std::vector<MessageG>> &nodes_l_,
                       const Channel &channel) const;

    double calcMa(const std::vector<NodeFlip> &flip,
                  std::vector<std::vector<MessageG>> &nodes_l_,
                  const Channel &channel) const;

    double calcFa(double val) const;

    Message cutCrc(const Message &message) const;

private:
    std::vector<bool> frozen_;
    double a_;
    int iters_;
    CrcPolarCode code_;
    SCBase *sc_;
};
