//
// Created by kranya on 14.04.2021.
//

#include "BchKernel.h"
#include <Polynom/Polynom.h>
#include <utils/utils.h>
#include <iostream>
#include <set>

Polynom<int> createPx(int n) {
    if (n == 2) {
        Polynom<int> ans(2);
        ans[0] = 1;
        ans[1] = 1;
        ans[2] = 1;
        return ans;
    } else if (n == 3) {
        Polynom<int> ans(3);
        ans[0] = 1;
        ans[1] = 1;
        ans[3] = 1;
        return ans;
    } else if (n == 4) {
        Polynom<int> ans(4);
        ans[0] = 1;
        ans[1] = 1;
        ans[4] = 1;
        return ans;
    } else if (n == 5) {
        Polynom<int> ans(5);
        ans[0] = 1;
        ans[2] = 1;
        ans[5] = 1;
        return ans;
    } else if (n == 6) {
        Polynom<int> ans(6);
        ans[0] = 1;
        ans[1] = 1;
        ans[6] = 1;
        return ans;
    } else if (n == 7) {
        Polynom<int> ans(7);
        ans[0] = 1;
        ans[3] = 1;
        ans[7] = 1;
        return ans;
    } else {
        std::cerr << "can't create px for this n" << std::endl;
        exit(0);
    }
}

std::vector<Polynom<int>> createField(int n) {
    auto px = createPx(getLog(n));
    std::vector<Polynom<int>> ans;

    Polynom<int> a = Polynom<int>::getPol(0);

    for (int i = 0; i < n-1; i++) {
        ans.push_back(a);
        Polynom prom = a * Polynom<int>::getPol(1);
        a = (prom) % px;
    }

    return ans;
}

std::vector<std::set<int>> buildCyclotomicClasses(int n) {
    std::vector<bool> check(n-1, false);
    std::vector<std::set<int>> ans;
    for (int i = 0; i < n-1; i++) {
        if (!check[i]) {
            int mul = 1;
            std::set<int> cur_set;
            for (int j = 0; j < n; j++) {
                if (!check[mul*i % (n-1)]) {
                    check[mul*i % (n-1) ] = true;
                    cur_set.insert(mul*i % (n-1));
                    mul *= 2;
                }
            }
            ans.emplace_back(cur_set);
        }
    }

    return ans;
}

Polynom<int> getGx(int n, int d, const std::vector<Polynom<int>>& field, const std::vector<std::set<int>>& cyclotomic) {
    std::vector<bool> need_cyclo(cyclotomic.size(), false);

    for (int i = 1; i < d-1; i++) {
        for (int j = 0; j < cyclotomic.size(); j++) {
            if (cyclotomic[j].contains(i)) {
                need_cyclo[j] = true;
                break;
            }
        }
    }

    Polynom< Polynom<int> > ans;
    ans.data_.resize(1);
    ans[0] = Polynom<int>::getPol(0);
    for (int j = 0; j < cyclotomic.size(); j++) {
        if (need_cyclo[j]) {
            for (auto &i: cyclotomic[j]) {
                Polynom<Polynom<int>> cur_pol;
                cur_pol.data_.resize(2);
                cur_pol.data_[0] = Polynom<int>::getPol(i);
                cur_pol.data_[1] = Polynom<int>::getPol(0);

                ans *= cur_pol;

            }
        }
    }

    Polynom<int> ans2;
    for (int i = 0; i < ans.data_.size(); i++) {
        Polynom<int> cur_pol;
        if (!ans.data_[i].data_.empty() && ans.data_[i].data_[0]) {
            cur_pol += Polynom<int>::getPol(0);
        }
        for (int j = 1; j < ans.data_[i].data_.size(); j++) {
            if (ans.data_[i].data_[j]) {
                cur_pol += field[j % (n-1)];
            }
        }
        ans2 += Polynom<int>::getPol(i) * cur_pol;
    }

    return ans2;
}

Matrix createExtendedBchKernel(int n) {

    if (n == 2) {
        Matrix ans(2, 2);
        ans[0][0] = 1;
        ans[0][1] = 0;
        ans[1][0] = 1;
        ans[1][1] = 1;
        return ans;
    }

    auto field = createField(n);

    auto cyclotomic = buildCyclotomicClasses(n);

    std::vector<Polynom<int>> bchs;

    for (int i = 1; i < cyclotomic.size(); i++) {
        bchs.emplace_back(getGx(n, (*cyclotomic[i].begin()), field, cyclotomic));
    }
    bchs.emplace_back(getGx(n, n-1, field, cyclotomic));

    int ind = 1;

    Matrix ans(n, n);

    for (int i = 0; i < bchs.size(); i++) {
        while (ind < n && (i+1 == bchs.size() || bchs[i+1].maxStep() != ind - 1)) {
            for (int j = 1; j < n; j++) {
                if (j-1 < bchs[i].data_.size()) {
                    ans[ind][j] = bchs[i].data_[j-1];
                } else {
                    ans[ind][j] = 0;
                }
            }
            ind++;
            bchs[i] *= Polynom<int>::getPol(1);
        }
    }

    ans[0][0] = 1;

    for (int i = 1; i < n; i++) {
        int sum = 0;
        for (int j = 1; j < n; j++) {
            sum += ans[i][j].get();
        }
        ans[i][0] = sum % 2;
    }

    return ans;
}