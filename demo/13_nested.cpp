// demo 13: nested expressions
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert(((arg1 + 1) * 2)(20) == 42);
    assert((((arg1 + 1) * 2) - 3)(20) == 39);
    assert(((arg1 < 10) && (arg1 > 0))(5));
    assert(!((arg1 < 10) && (arg1 > 0))(50));
    assert(((arg1 + arg2) * arg3)(1, 2, 3) == 9);
    std::cout << "demo 13 ok\n";
}
