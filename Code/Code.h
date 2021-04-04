#pragma once

#include <Message/Message.h>

class Code {
public:
    virtual Message encode(const Message& message) const = 0;
    virtual int getN() const = 0;
    virtual ~Code() = default;
};