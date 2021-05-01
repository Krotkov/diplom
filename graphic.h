#pragma once

#include <Code/PolarCode/PolarCode.h>
#include <Decoder/Decoder.h>
#include <utils/utils.h>
#include <future>
#include <Channel/Gaus/GausChannel.h>
#include <iostream>
#include <Decoder/SCs/SC/SC.h>
#include <Decoder/viterbi/Viterbi.h>

double build_graphic_step(const Code &code, GausChannel channel, const Decoder &decoder, int calc_iter, double x) {
    double ans = 0;
    channel.setNoise(x);
    for (int i = 0; i < calc_iter; i++) {
        if (i % 1000 == 0) {
            std::cout << x << " " << i << std::endl;
        }
        auto a = generateWord(code.getK());
        auto a1 = code.encode(a);

        auto b = channel.runMessage(a1);
        b = decoder.decode(b, channel);
        if (dynamic_cast<const Viterbi*>(&decoder)) {
            if (compare(a1, b) > 0) {
                ans += 1;
            }
        } else {
            if (compare(a, b) > 0) {
                ans += 1;
            }
        }
    }
    return ans / calc_iter;
}

std::vector<double> build_graphic(const Code &code, GausChannel &channel, const Decoder &decoder, int calc_iter,
                                  const std::vector<double> &sn) {
    std::vector<std::future<double>> results;
    results.reserve(sn.size());
    for (double i : sn) {
        results.push_back(
                std::async(build_graphic_step, std::cref(code), channel, std::cref(decoder), calc_iter, i));
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

void print_for_python(const std::vector<double> &a) {
    std::cout << "[";
    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i];
        if (i + 1 != a.size()) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
}