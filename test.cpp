#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlipViterbi.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>
#include <Decoder/SCs/SC/SC.h>
#include <Decoder/SCs/Old/SCFlip1.h>

int main() {
    int n = 4;
    int k = 2;

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    std::ifstream in2("../kernels/16-1-r.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    auto rKernel = Matrix(in2);

    GausChannel channel(n, k, 0);

    CrcPolarCode code(n, k, 1);
    SCFlip decoder1(code, 0.5, 10, false);
    SCFlip decoder2(code, 0.5, 10, true);

    Message a;
    for (int j = 0; j < k; j++) {
        a.add(0);
    }

    for (int i = 0; i < 10000; i++) {
        auto a1 = code.encode(a);
        auto b = channel.runMessage(a1);
        auto c1 = decoder1.decode(b, channel);
        auto c2 = decoder2.decode(b, channel);
        if (compare(c1, c2) != 0 && compare(c1, a) == 0) {
            a1.print();
            b.print();
            c1.print();
            c2.print();
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n";
        }
        std::cout << "\n----------------------------------------------------------\n";
    }

    return 0;
}