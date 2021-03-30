#pragma once

#include <Channel/Channel.h>

class PerfectGauss : public Channel {
public:

    Message runMessage(const Message& message) const override;

    double getLLR(const Symbol& symbol) const override;
};