#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>

class SCViterbi {
public:
    explicit SCViterbi(const PolarCodeWithLargeKernel& code);

    Message decode(const Message& message, const Channel& channel) const;

private:
    int n_;
    std::vector<bool> frozen_;
    Matrix kernel_;
};
