#pragma once

#include <Message/Message.h>
#include <Channel/Channel.h>
#include <Code/PolarCode/PolarCode.h>
#include <Decoder/Decoder.h>
#include <Decoder/viterbi/Viterbi.h>
#include <Decoder/SCs/SCBase.h>

class SCViterbi : public SCBase {
public:
    explicit SCViterbi(const PolarCode& code, bool useSpecialNodes = true);
protected:
    virtual double
    calculateLStep(const MessageG &y, const Message &u, const Channel &channel) const override;
private:
    std::vector<Viterbi> viterbiVect_;
};
