#pragma once

#include <vector>
#include <Decoder/Decoder.h>
#include "Code/PolarCode/PolarCode.h"
#include "Channel/Channel.h"

class SC : public Decoder {
public:
    SC(const PolarCode &code);

    Message decode(const Message &message, const Channel &channel) const override;

private:
    double
    calculateL(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel, int n,
               int i, int pref = 0) const;

    int n_;
    std::vector<bool> frozen_;
};
