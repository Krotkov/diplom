#pragma once

#include <vector>
#include <cassert>
#include <Symbol/Symbol.h>
#include <iostream>

template<typename T>
class MessageBase : public std::vector<T> {
public:
    MessageBase() : std::vector<T>() {};

    void add(const T &s) {
        this->push_back(s);
    }

    void print() {
        for (int i = 0; i < this->size(); i++) {
            std::cout << (*this)[i] << " ";
        }
        std::cout << "\n";
    }

    template<class Z>
    friend int compare(const MessageBase<Z> &a, const MessageBase<Z> &b);

    template<class Z>
    friend MessageBase<Z> operator+(const MessageBase<Z> &a, const MessageBase<Z> &b);

    template<class Z>
    friend MessageBase<Z> operator*(const MessageBase<Z> &a, const MessageBase<Z> &b);

    template<class Z>
    friend MessageBase<Z> operator*(const MessageBase<Z> &a, double val);

    MessageBase<T> &operator+=(const MessageBase<T> &other) {
        if (this->size() < other.size()) {
            this->resize(other.size(), 0);
        }
        for (int i = 0; i < other.size(); i++) {
            (*this)[i] += other[i];
        }
        return (*this);
    }

    MessageBase<T> &operator*=(const MessageBase<T> &other) {
        if (this->size() < other.size()) {
            this->resize(other.size(), 0);
        }
        for (int i = 0; i < other.size(); i++) {
            (*this)[i] *= other[i];
        }
        return (*this);
    }

    MessageBase<T> &operator*=(double val) {
        for (int i = 0; i < this->size(); i++) {
            (*this)[i] = (*this)[i].get() * val;
        }
        return (*this);
    }

    int getWeight() const {
        int ans = 0;
        for (int i = 0; i < this->size(); i++) {
            if ((*this)[i] == 1) {
                ans++;
            }
        }
        return ans;
    }
};

template<typename T>
int compare(const MessageBase<T> &a, const MessageBase<T> &b) {
    if (a.size() != b.size()) {
        return (int) (1e9);
    }
    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            ans++;
        }
    }
    return ans;
}

template<typename T>
MessageBase<T> operator+(const MessageBase<T> &a, const MessageBase<T> &b) {
    MessageBase ans(a);
    ans += b;
    return ans;
}

template<typename T>
MessageBase<T> operator*(const MessageBase<T> &a, const MessageBase<T> &b) {
    MessageBase ans(a);
    ans *= b;
    return ans;
}

template<typename T>
MessageBase<T> operator*(const MessageBase<T> &a, double val) {
    MessageBase ans(a);
    ans *= val;
    return ans;
}

using Message = MessageBase<Symbol>;

using MessageG = MessageBase<double>;