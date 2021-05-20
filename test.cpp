#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlipViterbi.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>
#include <Decoder/SCs/SC/SC.h>

int main() {
    int n = 16;
    int k = 8;

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    std::ifstream in2("../kernels/16-1-r.txt", std::ifstream::in);
    auto kernel = Matrix(in);
    auto rKernel = Matrix(in2);

    GausChannel channel(n, k, 5);
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
    PolarCode code(n, k, kernel, rKernel);
    SC decoder(code);

    auto a1 = generateWord(k);
//    a1[0] = 1;
//    a1[1] = 1;
//    a1.print();
    a1 = code.encode(a1);
//    a1.print();
    a1 = code.reverseEncode(a1);
    a1.print();

////    PolarCode code0(n, k, 0.5);
////    SC decoder0(code0);
////    for (int i = 0; i < 10000; i++) {
//    auto a1 = generateWord(k);
//    auto a2 = code.encode(a1);
//    auto a3 = channel.runMessage(a2);
////        a3.print();
//    auto a4 = decoder.decode(a3, channel);
//    a1.print();
//        a2.print();
//    a4.print();
//        a1.print();
//    }
    return 0;
}