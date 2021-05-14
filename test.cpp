#include <iostream>
#include <Code/CRC/CrcPolarCode.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <utils/utils.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <BCH/BchKernel.h>
#include <Decoder/SCs/SCFlip/SCFlipViterbi.h>
#include <Decoder/SCs/SCFlip/SCFlip.h>

int main() {
    int n = 256;
    int k = 128;

    std::ifstream in("../kernels/16-1.txt", std::ifstream::in);
    auto kernel = Matrix(in);

    GausChannel channel(n, k, 1);

    std::map<int, std::vector<int>> dynamicFrozen_;

    std::ifstream d("../kernels/frozen-1.txt", std::ifstream::in);
    while (true) {
        int num = 0;
        d >> num;
        std::cout << num << "\n";
        if (num == 0) {
            break;
        }
        std::vector<int> a;
        for (int i = 0; i < num - 1; i++) {
            int b;
            d >> b;
            a.emplace_back(b);
        }
        d >> num;
        dynamicFrozen_[num] = a;
    }

//    k -= 10;

    std::map<int, std::vector<int>> a1;
    CrcPolarCode code(n, k, kernel, a1, 10);
    auto frozen = code.getFrozen();

    for (int i = 0; i < frozen.size(); i++) {
        if (!frozen[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";

    SCFlipViterbi decoder(code, 0.5, 10);
    for (int i = 0; i < 10; i++) {
        auto a = generateWord(k);
//        a.print();

        Message b = code.encode(a);

        auto c = channel.runMessage(b);

        auto d = decoder.decode(c, channel);
//        d.print();
        if (compare(a, d) > 0) {
            std::cout << "difference ";
            for (int j = 0; j < a.size(); j++) {
                if (a[j] != d[j]) {
                    int ind = 0;
                    for (int q = 0; q < frozen.size(); q++) {
                        if (!frozen[q]) {
                            if (ind == j) {
                                std::cout << q << " ";
                            }
                            ind++;
                        }
                    }
                }
            }
            std::cout << "\n";
        }
        std::cout << "---------------------------------\n";
    }
    return 0;
}