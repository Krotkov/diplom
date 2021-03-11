#pragma once

class Symbol {
public:
    Symbol();

    Symbol(double val);

    int get();

    void addNoise(double val);

private:
    int value_;
};
