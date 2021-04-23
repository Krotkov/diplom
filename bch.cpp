#include <Matrix/Matrix.h>
#include <iostream>
#include <BCH/BchKernel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode/PolarCode.h>
#include <Decoder/SC/SC.h>

int main() {
    int n = 8;
    int k = 4;
    auto kernel = createExtendedBchKernel(8);

    kernel.print();

    PolarCodeWithLargeKernel code(n, k, 0.5, kernel);

    PolarCode code1(n, k, 0.5);
    SC decoder1(code1);

    SCViterbi decoder(code);
//    GausChannel channel(n, 2, 1);
    PerfectGauss channel;

    Message a = generateWord(k);
    a[0] = 1;
    a[1] = 0;
    a[2] = 1;
    a[3] = 1;
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