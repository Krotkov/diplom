#pragma once

class Code {
public:
    virtual int getN() const = 0;

    virtual int getK() const = 0;

    virtual Message encode(const Message& message) const = 0;

    virtual ~Code() = default;
};