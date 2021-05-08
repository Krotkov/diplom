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

    std::pair<Message, std::vector<double>>
    decodeStep(const MessageG &message, const Channel &channel, std::vector<std::vector<double>> &l_,
               std::vector<std::vector<Symbol>> &us, const std::vector<int> &flip) const;

    double calcMa(const std::vector<double> &l, const std::vector<int> &flip) const;

    Message cutCrc(const Message &message) const;

private:
    std::vector<bool> frozen_;
    double a_;
    int iters_;
    CrcPolarCode code_;
    SCBase *sc_;
};
