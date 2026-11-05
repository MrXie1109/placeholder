// demo 08: compound assignment
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    int x = 40;
    (arg1 += 2)(x);
    assert(x == 42);
    (arg1 *= 2)(x);
    assert(x == 84);
    (arg1 >>= 1)(x);
    assert(x == 42);
    (arg1 -= 2)(x);
    assert(x == 40);
    (arg1 |= 2)(x);
    assert(x == 42);
    std::cout << "demo 08 ok\n";
}
