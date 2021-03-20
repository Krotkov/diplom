#include <iostream>
#include "Channel/Channel.h"
#include "Code/PolarCode.h"

int main() {
    int n = 128, k = 64;
    double err = 0.5, noise = 5;
    Channel channel;
    Message a;
    for (int i = 0; i < k; i++) {
        a.add(Symbol(1));
    }
    std::cout << "message: ";
    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i].get() << " ";
    }
    std::cout << "\n";
    PolarCode code{n, k, err, noise};
    Message b = code.encode(a);

    std::cout << "encoded: ";
    for (int i = 0; i < b.size(); i++) {
        std::cout << b[i].get() << " ";
    }
    std::cout << "\n";
    Message c = channel.runMessage(b, n, k, noise);
    std::cout << "after channel: ";
    for (int i = 0; i < c.size(); i++) {
        std::cout << c[i].get() << " ";
    }
    std::cout << "\n";
    Message d = code.decode(c);
    std::cout << "decoded: ";
    for (int i = 0; i < d.size(); i++) {
        std::cout << d[i].get() << " ";
    }
    return 0;
}
