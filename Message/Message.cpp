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

int compare(const Message &a, const Message &b) {
    if (a.size() != b.size()) {
        return (int)(1e9);
    }
    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            ans++;
        }
    }
    return ans;
}

int Message::getWeight() {
    int ans = 0;
    for (int i = 0; i < this->size(); i++) {
        if ((*this)[i] == 1) {
            ans++;
        }
    }
    return ans;
}
