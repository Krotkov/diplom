#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>
#include <Decoder/SCs/SC/SC.h>

int main() {
    int n = 16;
    int k = 15;

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    std::ifstream in2("../kernels/16-1-r.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    auto rKernel = Matrix(in2);

    GausChannel channel(n, k, 0);

    PolarCode code1(n, k, kernel, rKernel);
//    SCViterbi decoder11(code1, false);
    SCViterbi decoder12(code1, true);

    auto a = generateWord(k);
    for (int i = 0; i < k; i++) {
        a[i] = 0;
    }
    a[0] = 1;
    a[1] = 1;
    a[6] = 1;
    a[7] = 1;
    auto b = code1.encode(a);
    auto c = channel.runMessage(b);
    auto d = decoder12.decode(c, channel);
    a.print();
    b.print();
    d.print();

    return 0;
}