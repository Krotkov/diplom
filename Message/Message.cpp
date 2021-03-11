//
// Created by kranya on 05.03.2021.
//

#include "Message.h"

void Message::add(const Symbol &s) {
    message_.push_back(s);
}

Message::Message(const Message &other) {
    message_ = other.message_;
}
