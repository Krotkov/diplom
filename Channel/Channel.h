#pragma once

#include <Message/Message.h>

class Channel {
public:
    virtual MessageG runMessage(const Message& message) const = 0;
    virtual double getLLR(const double& symbol) const = 0;

    virtual ~Channel() = default;
};