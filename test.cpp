#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlipViterbi.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>

int main() {
    int n = 256;
    int k = 128;

//    GausChannel channel(n, k, 1);
    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    auto kernel = Matrix(in);

    PerfectGauss channel;

    CrcPolarCode code(n, k, kernel, 10);

    SCFlip decoder(code, 0.5, 10);

    auto a = generateWord(k);
    a.print();

    Message b = code.encode(a);

    auto c = channel.runMessage(b);

    auto d = decoder.decode(c, channel);
    d.print();
    return 0;
}