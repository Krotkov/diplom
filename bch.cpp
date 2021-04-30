#include <Matrix/Matrix.h>
#include <iostream>
#include <BCH/BchKernel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/SCs/SCViterbi/SCViterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode/PolarCodeWithArikan.h>
#include <Decoder/SCs/SC/SC.h>

int main() {
   int n = 8;
    int k = 4;
    auto kernel = createExtendedBchKernel(2);

    kernel.print();

    PolarCodeWithLargeKernel code(n, k, 0.5, kernel);

    PolarCodeWithArikan code1(n, k, 0.5);
    SC decoder1(code1);

    SCViterbi decoder(code);
    PerfectGauss channel;

    Message a = generateWord(k);
    Message b = code.encode(a);
    std::cout << "coded= ";
    b.print();
    b = channel.runMessage(b);
    b = decoder.decode(b, channel);
    a.print();
    b.print();



    b = code1.encode(a);
    b = channel.runMessage(b);
    b = decoder1.decode(b, channel);
    return 0;
}