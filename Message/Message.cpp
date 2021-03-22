#include <iostream>
#include "Message.h"

void Message::add(const Symbol &s) {
    this->push_back(s);
}

void Message::print() {
    for (int i = 0; i < size(); i++) {
        std::cout << (*this)[i].get() << " ";
    }
    std::cout << "\n";
}
