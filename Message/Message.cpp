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

int Message::getWeight() const {
    int ans = 0;
    for (int i = 0; i < this->size(); i++) {
        if ((*this)[i] == 1) {
            ans++;
        }
    }
    return ans;
}

Message &Message::operator+=(const Message& other) {
    if (this->size() < other.size()) {
        this->resize(other.size(), 0);
    }
    for (int i = 0; i < other.size(); i++) {
        (*this)[i] += other[i];
    }
    return (*this);
}

Message operator+(const Message &a, const Message &b) {
    Message ans(a);
    ans += b;
    return ans;
}
