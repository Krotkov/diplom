#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>

int main() {
    int n = 4;
    int k = 2;
    PolarCode code(n, k, 0.5);

    Message a = generateWord(k);
//    a[0] = 1;
//    a[1] = 1;
//    a[1] = 0;
    a.print();
    a = code.encode(a);
    GausChannel channel(4, 2, 1);
    auto b = channel.runMessage(a);

    SC decoder(code);
    auto c = decoder.decode(b, channel);
    std::cout << "--------\n";
    c.print();
    return 0;
}