// demo 12: multiple placeholders
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((arg1 + arg2)(3, 4) == 7);
    assert((arg1 - arg2)(10, 4) == 6);
    assert((arg1 * arg2)(3, 4) == 12);
    assert((arg1 < arg2)(1, 2));
    assert((arg1 == arg2)(2, 2));
    assert((arg1 + arg2 + arg3)(1, 2, 3) == 6);
    std::cout << "demo 12 ok\n";
}
