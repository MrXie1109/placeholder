// demo 11: deferred function call
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int twice(int x) { return x * 2; }

int main() {
    assert(arg1(42)(twice) == 84);
    assert(arg1(1, 2)([](int a, int b) { return a + b; }) == 3);
    assert(arg2(42)(0, twice) == 84);
    assert((arg1(42) + 1)(twice) == 85);
    std::cout << "demo 11 ok\n";
}
