#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlipViterbi.h>

int main() {
    int n = 256;
    int k = 128;

//    PolarCode code(n, k, 0.5);
//    SC decoder(code);
//    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
//    auto kernel = Matrix(in);
    auto kernel = createExtendedBchKernel(4);
    CrcPolarCode code(n, k, kernel, 16);
    SCFlipViterbi decoder(code, 0);
    Message a = generateWord(k);
    auto a1 = code.encode(a);

//    GausChannel channel(4, 2, 1);
    PerfectGauss channel;
    auto b = channel.runMessage(a1);

    auto c = decoder.decode(b, channel);
    a.print();
    c.print();
//    }
    return 0;
}