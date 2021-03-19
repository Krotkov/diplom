#pragma once

#include <vector>
#include <cassert>
#include "../Symbol/Symbol.h"

class Message : public std::vector<Symbol> {
public:
    Message() : vector() {};

    void add(const Symbol &s);

};
