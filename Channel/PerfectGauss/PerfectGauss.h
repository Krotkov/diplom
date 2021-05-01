#pragma once

#include <Channel/Gaus/GausChannel.h>

class PerfectGauss : public GausChannel {
public:

    PerfectGauss();

    MessageG runMessage(const Message& message) const override;

    double getLLR(const double& symbol) const override;
};