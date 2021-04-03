#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>

class Decoder {
public:
    virtual Message decode(const Message& message, const Channel& channel) = 0;
    virtual ~Decoder() = default;
};