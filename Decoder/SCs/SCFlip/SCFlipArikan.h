#pragma once

#include <Decoder/SCs/SC/SC.h>
#include <Code/CRC/CrcPolarCode.h>

class SCFlipArikan : public SC {
public:
    explicit SCFlipArikan(const CrcPolarCode &code, int iters = 10);

    Message decode(const Message &message, const Channel &channel) const override;

    std::pair<Message, std::vector<double>>
    decodeStep(const Message &message, const Channel &channel, std::vector<std::vector<double>> &l_,
               const std::vector<int> &flip) const;

    double calcMa(const std::vector<double> &l, const std::vector<int> &flip) const;

    Message cutCrc(const Message &message) const;

private:
    double a_ = 0.5;
    CrcPolarCode code_;
    int iters_;
};

