#include "Message/Message.h"

bool checkIfPowerOfTwo(int n) {
    int a = 1;
    while (a < n) {
        a *= 2;
    }
    return a == n;
}

int getLog(int n) {
    int a = 1;
    int ans = 0;
    while (a < n) {
        a *= 2;
        ans++;
    }
    return ans;
}

Message generateWord(int n) {
    Message ans;
    for (int i = 0; i < n; i++) {
        ans.add(rand() % 2);
    }
    return ans;
}

int compareWords(const Message &a, const Message &b) {
    if (a.size() != b.size()) {
        return 1000000000;
    }
    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            ans++;
        }
    }
    return ans;
}