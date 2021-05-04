#pragma once

#include <Decoder/SCs/SC/SC.h>
#include <Code/CRC/CrcPolarCode.h>

class SCFlipArikan : public SC {
public:
    explicit SCFlipArikan(const CrcPolarCode &code, int iters = 10);

    Message decode(const MessageG &message, const Channel &channel) const override;

    std::pair<Message, std::vector<double>>
    decodeStep(const MessageG &message, const Channel &channel, std::vector<std::vector<double>> &l_,
               std::vector<std::vector<Symbol>> &us, const std::vector<int> &flip) const;

    double calcMa(const std::vector<double> &l, const std::vector<int> &flip) const;

    Message cutCrc(const Message &message) const;

private:
    double a_ = 0.5;
    CrcPolarCode code_;
    int iters_;
};

