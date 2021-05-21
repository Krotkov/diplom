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
    int n = 16;
    int k = 8;

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    std::ifstream in2("../kernels/16-1-r.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    auto rKernel = Matrix(in2);

    GausChannel channel(n, k, 10);
//    PerfectGauss channel;

//    std::map<int, std::vector<int>> dynamicFrozen_;
//
//    std::ifstream d("../kernels/frozen-1.txt", std::ifstream::in);
//    while (true) {
//        int num = 0;
//        d >> num;
//        std::cout << num << "\n";
//        if (num == 0) {
//            break;
//        }
//        std::vector<int> a;
//        for (int i = 0; i < num - 1; i++) {
//            int b;
//            d >> b;
//            a.emplace_back(b);
//        }
//        d >> num;
//        dynamicFrozen_[num] = a;
//    }
//    CrcPolarCode code(n, k, 1);
//    SCFlip decoder(code, 0.5, 10);
//    SCFlip1 decoder1(code, 0.5, 10);
//
//    for (int i = 0; i < 10000; i++) {
//        auto a1 = generateWord(k);
//        auto a2 = code.encode(a1);
//        auto a3 = channel.runMessage(a2);
//
//        auto a4 = decoder.decode(a3, channel);
//        auto a5 = decoder1.decode(a3, channel);
//        a1.print();
//        a2.print();
//        a4.print();
//        a5.print();
//        if (compare(a4, a5) > 0) {
//            return 0;
//        }
//        if (compare(a1, a4) != 0) {
//            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAa\n";
//        }
//        std::cout << "----------\n";
//    }

    PolarCode code(n, k, kernel, rKernel);
//    PolarCode code(n, k, 0.5);
    std::cout << "-------------------------------------------------------------------------\n";
    SCViterbi decoder(code);
    Message a = generateWord(k);
//    a.add(1);
//    a.add(1);
    a.print();
    a = code.encode(a);
    a.print();
    code.reverseEncode(a).print();
//    a = code.reverseEncode(a);
    auto b = channel.runMessage(a);
    b.print();
    auto c = decoder.decode(b, channel);
    c.print();
    return 0;
}