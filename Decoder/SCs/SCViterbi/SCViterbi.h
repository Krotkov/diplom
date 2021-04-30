#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>
#include <Code/PolarCode.h>
#include <Decoder/Decoder.h>
#include <Decoder/viterbi/Viterbi.h>
#include <Decoder/SCs/SCBase.h>

class SCViterbi : public SCBase {
public:
    explicit SCViterbi(const PolarCode& code);
protected:
    virtual double
    calculateLStep(std::vector <std::vector<double>> &l_, const Message &y, const Message &u, const Channel &channel,
                   int n,
                   int i, int pref = 0) const override;
private:
    std::vector<Viterbi> viterbiVect_;
};
