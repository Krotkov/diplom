#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>

int main() {
    int n = 8;
    int k = 4;
    PolarCode code(n, k, 0.5);
    SC decoder(code);

    PerfectGauss channel;

    Message a = generateWord(k);
    a[0] = 1;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a.print();
    Message b = code.encode(a);
    b = channel.runMessage(b);
    b = decoder.decode(b, channel);
    b.print();
    return 0;
}