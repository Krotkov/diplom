#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>

int main() {
    int n = 1024;
    int k = 512;
    PolarCode code(n, k, 0.5);
    SC decoder(code);

    PerfectGauss channel;

    for (int i = 0; i < 10000; i++) {
        Message a = generateWord(k);
        a.print();
        Message b = code.encode(a);
        b = channel.runMessage(b);
        b = decoder.decode(b, channel);
        b.print();
    }
    return 0;
}