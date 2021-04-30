//
// Created by kranya on 01.05.2021.
//

#include <Message/Message.h>
#include "Polynom.h"

Polynom<int> polFromMessage(const Message &message) {
    Polynom<int> ans((int) message.size() - 1);
    for (int i = 0; i < message.size(); i++) {
        ans[i] = message[i].get();
    }
    return ans;
}

