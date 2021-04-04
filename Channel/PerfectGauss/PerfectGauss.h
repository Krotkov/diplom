#pragma once

#include <Channel/Gaus/GausChannel.h>

class PerfectGauss : public GausChannel {
public:

    PerfectGauss();

    Message runMessage(const Message& message) const override;

    double getLLR(const Symbol& symbol) const override;
};