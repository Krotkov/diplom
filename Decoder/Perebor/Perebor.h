#pragma once

#include <Code/Code.h>
#include <Decoder/Decoder.h>

class Perebor : public Decoder {
public:
    explicit Perebor(const Code& code);
    Message decode(const Message& message, const Channel& channel) const override;
private:
    std::vector<Message> codeWords_;
};
