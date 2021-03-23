#pragma once

#include <vector>
#include "Code/PolarCode.h"
#include "Channel/Channel.h"

class SC {
public:
    SC(const PolarCode& code, double noise);

    Message decode(const Message& message);

private:
    double calculateL(const Message& y, const Message& u, int n, int i, int pref=0);

    int n_;
    int k_;
    double noise_;
    std::vector<std::vector<double>> l_;
    std::vector<bool> frozen_;
};
