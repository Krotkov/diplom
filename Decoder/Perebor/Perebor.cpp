//
// Created by kranya on 04.04.2021.
//

#include "Perebor.h"

Perebor::Perebor(const PolarCode &code) {
    int k = code.getK();
    for (int i = 0; i < (1 << k); i++) {
        Message a;
        for (int j = 0; j < k; j++) {
            a.add(Symbol((i >> j) & 1));
        }
        Message coded = code.encode(a);
        codeWords_.push_back(coded);
    }
}

Message Perebor::decode(const MessageG &message, const Channel &channel) const {
    double max = 0;
    int ans = 0;
    for (int i = 0; i < codeWords_.size(); i++) {
        double cur = 0;
        for (int j = 0; j < codeWords_[i].size(); j++) {
            if (codeWords_[i][j] == 1) {
                cur -= channel.getLLR(message[j]);
            } else {
                cur += channel.getLLR(message[j]);
            }
        }
        if (cur > max) {
            max = cur;
            ans = i;
        }
    }
    return codeWords_[ans];
}
