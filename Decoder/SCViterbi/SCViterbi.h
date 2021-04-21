#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>
#include <Code/PolarCodeWithLargeKernel/PolarCodeWithLargeKernel.h>
#include <Decoder/Decoder.h>
#include <Decoder/viterbi/Viterbi.h>

class SCViterbi : public Decoder {
public:
    explicit SCViterbi(const PolarCodeWithLargeKernel& code);

    Message decode(const Message& message, const Channel& channel) const override;

    double
    calculateL(std::vector<std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel, int n,
               int i, int pref = 0) const;

private:
    int n_;
    std::vector<bool> frozen_;
    Matrix kernel_;
    std::vector<Viterbi> viterbiVect_;
};
