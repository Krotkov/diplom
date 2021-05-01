#include <iostream>
#include <Code/CRC/CrcPolarCode.h>

int main() {
    int n = 64;
    int k = 32;
    CrcPolarCode code(n, k);

    Message a;
    for (int i = 0; i < 32; i++) {
        a.add(0);
    }
    a[16] = 1;
    a[15] = 1;
    code.encode(a);
    return 0;
}