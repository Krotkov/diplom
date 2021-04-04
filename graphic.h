#pragma once

#include <Code/Code.h>
#include <Decoder/Decoder.h>
#include <utils/utils.h>
#include <future>
#include <Channel/Gaus/GausChannel.h>

double build_graphic_step(const Code& code, GausChannel& channel, const Decoder& decoder, int calc_iter, double x) {
    double ans = 0;
    channel.setNoise(x);
    for (int i = 0; i < calc_iter; i++) {
        auto a = generateWord(code.getN());
        a = code.encode(a);

        auto b = channel.runMessage(a);
        b = decoder.decode(b, channel);
        if (compare(a, b) > 0) {
            ans += 1;
        }
    }
    return ans / calc_iter;
}

std::vector<double> build_graphic(const Code &code, GausChannel &channel, const Decoder &decoder, int calc_iter,
                                  const std::vector<double>& sn) {
    std::vector<std::future<double>> results;
    results.reserve(sn.size());
    for (int i = 0; i < sn.size(); i++) {
        results.push_back(std::async(build_graphic_step, std::cref(code), std::ref(channel), std::cref(decoder), calc_iter, sn[i]));
    }

    for (int i = 0; i < results.size(); i++) {
        results[i].wait();
    }
//    return ans;
}