#pragma once

#include <Message/Message.h>

class Channel {
public:
    virtual Message runMessage(const Message& message) const = 0;
    virtual double getLLR(const Symbol& symbol) const = 0;
};