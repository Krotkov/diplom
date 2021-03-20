#pragma once

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