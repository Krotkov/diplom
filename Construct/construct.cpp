

#include <vector>
#include "construct.h"

double calculateZ(int n, int i, double err) {
    if (n == 1 && i == 1) {
        return err;
    }
    if (i % 2 == 0) {
        auto z = calculateZ(n / 2, i / 2, err);
        return z * z;
    } else {
        auto z = calculateZ(n / 2, (i + 1) / 2, err);
        return 2 * z - z * z;
    }
}

std::vector<bool> constructCode(int n, int k, double err) {
    std::vector<std::pair<double, int>> zz(n);
    for (int i = 0; i < n; i++) {
        zz[i] = {calculateZ(n, i + 1, err), i + 1};
    }
    std::sort(zz.begin(), zz.end());

    std::vector<bool> ans(n, false);
    for (int i = 0; i < k; i++) {
        ans[zz[i].second] = true;
    }

    return ans;
}

