#pragma once

#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>

class SCFlipViterbi : public SCViterbi {
public:
    explicit SCFlipViterbi(const CrcPolarCode &code, double a = 0.5, int iters = 10);

    Message decode(const MessageG &message, const Channel &channel) const override;

    std::pair<Message, std::vector<double>>
    decodeStep(const MessageG &message, const Channel &channel, const std::vector<int> &flip) const;

    double calcMa(const std::vector<double> &l, const std::vector<int> &flip) const;

    Message cutCrc(const Message &message) const;

private:
    double a_ = 0.5;
    CrcPolarCode code_;
    int iters_;
};

