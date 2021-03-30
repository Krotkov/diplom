#pragma once

#include <vector>
#include "Code/PolarCode.h"
#include "Channel/Channel.h"

class SC {
public:
    SC(const PolarCode& code);

    Message decode(const Message& message, const Channel& channel);

private:
    double calculateL(const Message& y, const Message& u, const Channel& channel, int n, int i, int pref=0);

    int n_;
    std::vector<std::vector<double>> l_;
    std::vector<bool> frozen_;
};
