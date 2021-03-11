//
// Created by kranya on 05.03.2021.
//

#include "Symbol.h"

Symbol::Symbol() {
    value_ = 0;
}

Symbol::Symbol(double val) {
    if (val > 0) {
        value_ = 1;
    } else {
        value_ = 0;
    }
}

int Symbol::get() {
    return value_;
}

void Symbol::addNoise(double noise) {
    if ((double)value_ + noise > 1) {
        value_ = 1;
    } else {
        value_ = 0;
    }
}
