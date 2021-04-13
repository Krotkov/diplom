#include <Matrix/Matrix.h>
#include <iostream>
#include <Polynom/Polynom.h>

Matrix shortering(const Matrix &a) {
    Matrix ans(a.getN() - 1, a.getK() - 1);
    int max_column = -1, max_last_1 = int(1e9);
    for (int j = 0; j < a.getK(); j++) {
        int last_1 = -1;
        for (int i = 0; i < a.getN(); i++) {
            if (a[i][j] == 1) {
                last_1 = i;
            }
        }
        if (max_last_1 > last_1) {
            max_column = j;
            max_last_1 = last_1;
        }
    }
    for (int i = 0; i < a.getN(); i++) {
        for (int j = 0; j < a.getK(); j++) {
            if (j != max_column && i != max_last_1) {
                int curi = i < max_last_1 ? i : i - 1;
                int curj = j < max_column ? j : j - 1;
                if (a[i][max_column] == 1) {
                    ans[curi][curj] = a[i][j] + a[max_last_1][j];
                } else {
                    ans[curi][curj] = a[i][j];
                }
            }
        }
    }
    return ans;
}

int main() {
    Polynom<int> pol(4);
    pol[0] = 1;
    pol[1] = 1;
    pol[4] = 1;

    Polynom<int> a = Polynom<int>::getPol(1);

    for (int i = 0; i < 16; i++) {
        a.print();
        Polynom prom = a * Polynom<int>::getPol(1);
//        std::cout << "prom= ";
//        prom.print();
        a = (prom) % pol;
    }
}