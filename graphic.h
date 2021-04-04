#pragma once

#include <Code/Code.h>
#include <Decoder/Decoder.h>
#include <utils/utils.h>
#include <future>
#include <Channel/Gaus/GausChannel.h>
#include <iostream>

double build_graphic_step(const Code &code, GausChannel &channel, const Decoder &decoder, int calc_iter, double x) {
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
                                  const std::vector<double> &sn) {
    std::vector<std::future<double>> results;
    results.reserve(sn.size());
    for (double i : sn) {
        GausChannel channel1(channel);
        results.push_back(
                std::async(build_graphic_step, std::cref(code), std::ref(channel), std::cref(decoder), calc_iter, i));
    }

    for (auto &result : results) {
        result.wait();
    }
    std::vector<double> ans;
    ans.reserve(results.size());
    for (auto &result : results) {
        ans.push_back(result.get());
    }
    return ans;
}