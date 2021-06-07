#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>

class Decoder {
public:
    virtual Message decode(const MessageG &message, const Channel &channel) const = 0;

    virtual ~Decoder() = default;
};