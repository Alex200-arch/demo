#include <iostream>

int main() {
    int a = -9;
    int b = 8;
    bool same_sign = (a > 0 == b > 0);
    std::cout << "same sign " << same_sign << "\n";
    return 0;
}

