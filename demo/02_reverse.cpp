// demo 02: placeholder on the right-hand side
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((1 + arg1)(41) == 42);
    assert((100 - arg1)(58) == 42);
    assert((2 * arg1)(21) == 42);
    assert((84 / arg1)(2) == 42);
    assert((47 % arg1)(5) == 2);
    std::cout << "demo 02 ok\n";
}
