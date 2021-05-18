#pragma once

#include <vector>
#include <Code/PolarCode/PolarCode.h>
#include <Decoder/Decoder.h>
#include <Decoder/SCs/SCBase.h>
#include "Channel/Channel.h"

class SC : public SCBase {
public:
    SC(const PolarCode &code);
protected:
    virtual double
    calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const override;
};
