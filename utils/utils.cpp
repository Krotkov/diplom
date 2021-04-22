#include <iostream>
#include "Message/Message.h"

bool checkIfPowerOfTwo(int n) {
    int a = 1;
    while (a < n) {
        a *= 2;
    }
    return a == n;
}

int getLog(int n, int base) {
    int a = 1;
    int ans = 0;
    while (a < n) {
        a *= base;
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

int pow(int a, int b) {
    int ans = 1;
    for (int i = 0; i < b; i++) {
        ans *= a;
    }
    return ans;
}

long long CnK(long long a, long long b) {
    long long ans = 1;
    for (long long i = b+1; i <= a; i++) {
        ans *= i;
    }
    for (long long i = 2; i <= (a-b); i++) {
        ans /= i;
    }
    return ans;
}