//
// Created by kranya on 19.04.2021.
//

#include "SCViterbi.h"

Message SCViterbi::decode(const Message &message, const Channel &channel) const {
    return Message();
}

SCViterbi::SCViterbi(const PolarCodeWithLargeKernel &code) {
    n_ = code.getN();
    frozen_ = code.getFrozen();
    kernel_ = code.getKernel();
}
