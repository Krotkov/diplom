#include <Matrix/Matrix.h>
#include <iostream>
#include <BCH/BchKernel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode/PolarCode.h>

int main() {
    int n = 64;
    auto kernel = createExtendedBchKernel(8);

    kernel.print();

    PolarCodeWithLargeKernel code(n, 1, 0.5, kernel);

    PolarCode code1(n, 1, 0.5);

    return 0;

    SCViterbi decoder(code);
    GausChannel channel(n, 3, 1);

    Message a = generateWord(3);
    Message b = code.encode(a);
    std::cout << "coded= ";
    b.print();
    b = channel.runMessage(b);
    b = decoder.decode(b, channel);
    a.print();
    b.print();
    return 0;
}