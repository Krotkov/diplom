//
// Created by kranya on 06.05.2021.
//

#pragma once


#include <Decoder/Decoder.h>
#include <Decoder/SCs/SCBase.h>
#include <Code/CRC/CrcPolarCode.h>

class SCFlip : public Decoder {
public:
    SCFlip(const CrcPolarCode &code, double a = 0.5, int iters = 10);

    ~SCFlip();

    Message decode(const MessageG &message, const Channel &channel) const override;

    Message
    decodeStep(const MessageG &message, const Channel &channel, const std::vector<NodeFlip> &flip,
               std::vector<std::vector<MessageG>> &nodes_l_,
               std::vector<std::vector<std::vector<bool>>> &flips_) const;

    double calcMa(const std::vector<NodeFlip> &flip,
                  const std::vector<std::vector<MessageG>> &nodes_l_) const;

    double calcFa(double val) const;

    Message cutCrc(const Message &message) const;

private:
    std::vector<bool> frozen_;
    double a_;
    int iters_;
    CrcPolarCode code_;
    SCBase *sc_;
};
