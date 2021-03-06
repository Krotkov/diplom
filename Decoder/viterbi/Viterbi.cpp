//
// Created by kranya on 23.03.2021.
//

#include <Channel/Channel.h>
#include "Viterbi.h"
#include "utils/utils.h"

int getFirst(const std::vector<Symbol> &x) {
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == Symbol(1)) {
            return i;
        }
    }
    return -1;
}

int getLast(const std::vector<Symbol> &x) {
    for (int i = int(x.size()) - 1; i >= 0; i--) {
        if (x[i] == Symbol(1)) {
            return i;
        }
    }
    return -1;
}

void toSpanForm(Matrix &matrix) {
    auto spanComp = [](const std::vector<Symbol> &a, const std::vector<Symbol> &b) {
        return getFirst(a) < getFirst(b);
    };

    std::sort(matrix.begin(), matrix.end(), spanComp);

    //make different beginnings
    for (int q = 0; q < matrix.getK(); q++) {
        int cur_ind = 0;
        for (int i = 1; i < matrix.getN(); i++) {
            if (getFirst(matrix[i]) == getFirst(matrix[cur_ind])) {
                for (int j = 0; j < matrix.getK(); j++) {
                    matrix[i][j] += matrix[i - 1][j];
                }
            } else {
                cur_ind = i;
            }
        }
        std::sort(matrix.begin(), matrix.end(), spanComp);
    }

    //make different endings
    for (int w = 0; w < matrix.getK(); w++) {
        std::vector<std::vector<int>> ends;
        ends.resize(matrix.getK());
        for (int i = 0; i < matrix.getN(); i++) {
            ends[getLast(matrix[i])].push_back(i);
        }
        for (auto &end : ends) {
            if (end.size() >= 2) {
                for (int j = 0; j + 1 < end.size(); j++) {
                    for (int q = 0; q < matrix.getK(); q++) {
                        matrix[end[j]][q] += matrix[end.back()][q];
                    }
                }
            }
        }
    }

    std::sort(matrix.begin(), matrix.end(), spanComp);
}

Symbol sumMessage(const Message &a, const Message &b) {
    assert(a.size() == b.size());

    Symbol ans = 0;
    for (int i = 0; i < a.size(); i++) {
        ans += (a[i] * b[i]);
    }

    return ans;
}

void Viterbi::build_grid() {
    grid_.resize(matrix_.getK() + 1);
    grid_[0].resize(1);

    //make grid shape
    int openedNum = 0;
    bool hasOneSymbolStr = false;
    for (int i = 0; i < matrix_.getK(); i++) {
        for (int j = 0; j < matrix_.getN(); j++) {
            int first = getFirst(matrix_[j]);
            if (first == i) {
                openedNum++;
            }
            int last = getLast(matrix_[j]);
            if (last == i) {
                openedNum--;
            }
            if (first == last) {
                hasOneSymbolStr = true;
            }
        }
        int layout_size = pow(2, openedNum + (hasOneSymbolStr ? 1 : 0));
        grid_[i + 1].resize(layout_size);
    }

    std::vector<bool> opened(matrix_.getN(), false);
    for (int i = 0; i < matrix_.getK(); i++) {
        int closeOnNext = -1, openOnNext = -1;
        for (int j = 0; j < matrix_.getN(); j++) {
            if (getFirst(matrix_[j]) == i) {
                openOnNext = j;
            }
            if (getLast(matrix_[j]) == i) {
                closeOnNext = j;
            }
        }

        for (int j = 0; j < grid_[i].size(); j++) {
            grid_[i][j] = Node();
            int newIndexTemplate = 0;
            int ind = 0;
            int curIndex = j;
            int closedBit = 0;
            for (int q = 0; q < matrix_.getN(); q++) {
                if (opened[q]) {
                    if (closeOnNext != q) {
                        newIndexTemplate += ((curIndex & 1) << ind);
                        ind++;
                    } else {
                        closedBit = (curIndex & 1);
                    }
                    curIndex >>= 1;
                }
            }

            auto column = matrix_.getColumn(i);

            Message gridColumn;
            gridColumn.resize(column.size(), 0);
            ind = 0;
            for (int q = 0; q < opened.size(); q++) {
                if (opened[q] && closeOnNext != q) {
                    gridColumn[q] = Symbol((newIndexTemplate >> ind) & 1);
                    ind++;
                }
            }

            if (openOnNext != -1) {
                grid_[i][j].next_0 = newIndexTemplate + (0 << ind);
                grid_[i][j].s_0 = sumMessage(gridColumn, column) + closedBit;

                gridColumn[openOnNext] = Symbol(1);
                grid_[i][j].next_1 = newIndexTemplate + (1 << ind);
                grid_[i][j].s_1 = sumMessage(gridColumn, column) + closedBit;
            } else {
                grid_[i][j].next_0 = newIndexTemplate;
                grid_[i][j].s_0 = sumMessage(gridColumn, column) + closedBit;
            }
        }

        if (openOnNext != -1) {
            opened[openOnNext] = true;
        }
        if (closeOnNext != -1) {
            opened[closeOnNext] = false;
        }
    }

}

Viterbi::Viterbi(const Matrix &matrix) {
    matrix_ = matrix;
    toSpanForm(matrix_);
    build_grid();
}

Message Viterbi::decode(const MessageG &message, const Channel &channel) const {

    auto dp = count_dp(message, channel);

    Message ans;
    int indJ = 0, indI = dp.size() - 1;
    if (dp.back().size() > 1 && dp.back()[1].first > dp.back()[0].first) {
        indJ = 1;
    }
    for (int i = 1; i < dp.size(); i++) {
        int newIndJ = dp[indI][indJ].second;
        indI--;
        if (grid_[indI][newIndJ].next_0 == indJ) {
            ans.add(grid_[indI][newIndJ].s_0);
        } else {
            ans.add(grid_[indI][newIndJ].s_1);
        }
        indJ = newIndJ;
    }

    std::reverse(ans.begin(), ans.end());
    return ans;
}

Viterbi &Viterbi::operator=(const Viterbi &other) {
    this->grid_ = other.grid_;
    this->matrix_ = other.matrix_;

    return (*this);
}

Viterbi &Viterbi::operator=(Viterbi &&other) noexcept {
    this->grid_ = other.grid_;
    this->matrix_ = other.matrix_;

    return (*this);
}

std::vector<std::vector<std::pair<double, int>>>
Viterbi::count_dp(const MessageG &message, const Channel &channel) const {
    std::vector<std::vector<std::pair<double, int>>> dp(grid_.size());

    for (int i = 0; i < dp.size(); i++) {
        dp[i].resize(grid_[i].size(), {-1e9, -1});
    }

    dp[0][0].first = 0;

    for (int i = 0; i + 1 < dp.size(); i++) {
        for (int j = 0; j < dp[i].size(); j++) {
            double value = message[i];
            double s0_value = value * (grid_[i][j].s_0 == 1 ? -1 : 1);
            if (s0_value > 0) {
                s0_value = 0;
            }
            if (dp[i + 1][grid_[i][j].next_0].first < dp[i][j].first + s0_value) {
                dp[i + 1][grid_[i][j].next_0].first = dp[i][j].first + s0_value;
                dp[i + 1][grid_[i][j].next_0].second = j;
            }
            double s1_value = value * (grid_[i][j].s_1 == 1 ? -1 : 1);
            if (s1_value > 0) {
                s1_value = 0;
            }
            if (grid_[i][j].next_1 != -1 && dp[i + 1][grid_[i][j].next_1].first < dp[i][j].first + s1_value) {
                dp[i + 1][grid_[i][j].next_1].first = dp[i][j].first + s1_value;
                dp[i + 1][grid_[i][j].next_1].second = j;
            }
        }
    }
    return dp;
}

double Viterbi::calcLLr(const MessageG &message, const Channel &channel) const {
    auto dp = count_dp(message, channel);

    if (dp.back().size() > 1) {
        return dp.back()[0].first - dp.back()[1].first;
    } else {
        return (*(dp.end() - 2))[0].first - (*(dp.end() - 2))[1].first;
    }
}

Viterbi::Viterbi(const Viterbi &viterbi) {
    this->grid_ = viterbi.grid_;
    this->matrix_ = viterbi.matrix_;
}
