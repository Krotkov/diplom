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

void Message::mulOnMatrix(const Matrix &matrix) {
    Message message;
    for (int j = 0; j < matrix.getK(); j++) {
        message.add(0);
        for (int q = 0; q < matrix.getN(); q++) {
            message[j] += (*this)[q] * matrix[q][j];
        }
    }
    std::swap(*this, message);
}
