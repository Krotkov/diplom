#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Decoder/SCs/SCFlip/SCFlipArikan.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>

int main() {
    int n = 4;
    int k = 2;

//    PolarCode code(n, k, 0.5);
//    SC decoder(code);
//    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
//    auto kernel = Matrix(in);
    auto kernel = createExtendedBchKernel(4);
    PolarCode code(n, k, kernel);
    SCViterbi decoder(code);
//    for (int i = 0; i < 10000; i++) {
    Message a = generateWord(k);
//    a[0] = 0;
//    a[1] = 1;
//    a[2] = 0;
//    a[3] = 0;
//        a.print();
    auto a1 = code.encode(a);

    GausChannel channel(4, 2, 1);
    auto b = channel.runMessage(a1);

    auto c = decoder.decode(b, channel);
    a.print();
    c.print();
//    }
    return 0;
}