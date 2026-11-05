// demo 20: build an expression once, reuse it with different arguments
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    auto add = arg1 + arg2;
    assert(add(1, 2) == 3);
    assert(add(10, 20) == 30);

    auto square = arg1 * arg1;
    assert(square(7) == 49);

    auto clamp01 = (arg1 < 0) || (arg1 > 1);
    assert(clamp01(-1));
    assert(!clamp01(0));

    std::cout << "demo 20 ok\n";
}
