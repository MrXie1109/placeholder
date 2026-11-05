// demo 16: store expressions in std::function
#include "../placeholder.hpp"
#include <cassert>
#include <functional>
#include <iostream>

using namespace placeholder;

int main() {
    std::function<int(int)> f = 100 - arg1;
    assert(f(58) == 42);

    std::function<int(int, int)> g = arg1 + arg2;
    assert(g(3, 4) == 7);

    std::function<int(int)> h = arg1(42)([](int x) { return x; }) + arg1;
    assert(h(0) == 42);

    std::cout << "demo 16 ok\n";
}
