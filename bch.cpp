#include <Matrix/Matrix.h>
#include <iostream>
#include <Polynom/Polynom.h>
#include <BCH/BchKernel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>

int main() {
    int n = 64;
    auto kernel = createExtendedBchKernel(4);

    PolarCodeWithLargeKernel code(n, n, 0.5, kernel);
    SCViterbi decoder(code);
    PerfectGauss channel;

    Message a = generateWord(n);
    Message b = code.encode(a);
    std::cout << "coded= ";
    b.print();
    b = channel.runMessage(b);
    b = decoder.decode(b, channel);
    a.print();
    b.print();
    return 0;
}