// demo 14: const and non-const expression objects
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    auto f = arg1 + 1;
    assert(f(41) == 42);

    const auto g = arg1 + 1;
    assert(g(41) == 42);

    const auto h = arg1 + arg2;
    assert(h(1, 2) == 3);

    std::cout << "demo 14 ok\n";
}
