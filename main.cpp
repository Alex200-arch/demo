#include <iostream>
#include "common-lib/types.hpp"

int main() {
    auto a = LongDecimal<18>::FromRaw({1,2});
    std::cout << "a " << a.ToDouble() << "\n";
    std::cout << "a " << a.ToString() << "\n";
    return 0;
}

