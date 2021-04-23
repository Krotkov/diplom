#include <Matrix/Matrix.h>
#include <iostream>
#include <BCH/BchKernel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/SCViterbi/SCViterbi.h>
#include <Channel/PerfectGauss/PerfectGauss.h>
#include <Code/PolarCode/PolarCode.h>
#include <Decoder/SC/SC.h>

int main() {
//    Matrix kern(16, 16);
//    for (int i = 0; i < 16; i++) {
//        kern[i][0] = 1;
//        for (int j = 1; j < 16; j++) {
//            kern[i][j] = 0;
//        }
//    }
//    for (int i = 0; i < 16; i++) {
//        kern[15][i] = 1;
//    }
//    kern[1][9] = 1;
//    kern[2][11] = 1;
//    kern[3][4] = 1;
//    kern[4][3] = 1;
//    kern[5][2] = 1;
//    kern[5][9] = 1;
//    kern[5][11] = 1;
//    kern[6][4] = 1;
//    kern[6][11] = 1;
//    kern[6][15] = 1;
//    kern[7][3] = 1;
//    kern[7][4] = 1;
//    kern[7][7] = 1;
//    kern[8][1] = 1;
//    kern[8][2] = 1;
//    kern[8][3] = 1;
//    kern[9][1] = 1;
//    kern[9][2] = 1;
//    kern[9][7] = 1;
//    kern[9][9] = 1;
//    kern[9][13] = 1;
//    kern[10][1] = 1;
//    kern[10][8] = 1;
//    kern[10][11] = 1;
//    kern[10][13] = 1;
//    kern[10][15] = 1;
//    kern[11][2] = 1;
//    kern[11][4] = 1;
//    kern[11][6] = 1;
//    kern[11][9] = 1;
//    kern[11][11] = 1;
//    kern[11][13] = 1;
//    kern[11][15] = 1;
//    kern[12][3] = 1;
//    kern[12][4] = 1;
//    kern[12][7] = 1;
//    kern[12][8] = 1;
//    kern[12][11] = 1;
//    kern[12][12] = 1;
//    kern[12][15] = 1;
//    for (int i = 1; i <= 7; i++) {
//        kern[13][i] = 1;
//    }
//    kern[14][1] = 1;
//    kern[14][2] = 1;
//    kern[14][3] = 1;
//    kern[14][12] = 1;
//    kern[14][13] = 1;
//    kern[14][14] = 1;
//    kern[14][15] = 1;





    int n = 8;
    int k = 4;
    auto kernel = createExtendedBchKernel(2);

    kernel.print();

    PolarCodeWithLargeKernel code(n, k, 0.5, kernel);

    PolarCode code1(n, k, 0.5);
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