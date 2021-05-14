#pragma once

#include <Code/PolarCode/PolarCode.h>
#include <Decoder/Decoder.h>
#include <utils/utils.h>
#include <future>
#include <Channel/Gaus/GausChannel.h>
#include <iostream>
#include <Decoder/SCs/SC/SC.h>
#include <Decoder/viterbi/Viterbi.h>

double build_graphic_step(const Code &code, const GausChannel &channel, const Decoder &decoder, int calc_iter, double x,
                          int idx) {
    double ans = 0;
    for (int i = 0; i < calc_iter; i++) {
        if (idx == 0 && i % 1000 == 0) {
            std::cout << x << " " << i << "\n";
        }
        auto a = generateWord(code.getK());
        auto a1 = code.encode(a);

        auto b = channel.runMessage(a1);
        auto c = decoder.decode(b, channel);
        if (dynamic_cast<const Viterbi *>(&decoder)) {
            if (compare(a1, c) > 0) {
                ans += 1;
            }
        } else {
            if (compare(a, c) > 0) {
                ans += 1;
            }
        }
    }
//    std::cout << x << " " << idx << "\n";
    return ans / calc_iter;
}

std::vector<double> build_graphic(const Code &code, GausChannel &channel, const Decoder &decoder, int calc_iter,
                                  const std::vector<double> &sn) {
    int threadCount = 20;

    std::vector<double> ans;
    ans.reserve(sn.size());

    for (double i : sn) {
        channel.setNoise(i);

        std::vector<std::future<double>> results;
        results.reserve(threadCount);
        for (int j = 0; j < threadCount; j++) {
            results.push_back(std::async(build_graphic_step, std::cref(code), std::cref(channel), std::cref(decoder),
                                         calc_iter / threadCount, i, j));
        }

        double sum = 0;
        for (int j = 0; j < threadCount; j++) {
            results[j].wait();
            sum += results[j].get();
        }
        std::cout << "done: " << i << "\n";
        ans.push_back(sum / threadCount);
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