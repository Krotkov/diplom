#pragma once

#include <vector>
#include <cassert>
#include "../Symbol/Symbol.h"

class Message {
public:
    Message() = default;

    Message(const Message& other);

    Symbol operator[](int ind) const {
        assert(ind < message_.size());
        return message_[ind];
    }

    Symbol& operator[](int ind) {
        assert(ind < message_.size());
        return message_[ind];
    }

    void add(const Symbol& s);

    int size() {
        return message_.size();
    }

private:
    std::vector<Symbol> message_;
};
