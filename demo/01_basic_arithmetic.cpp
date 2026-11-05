// demo 01: basic arithmetic on arg1
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((arg1 + 1)(41) == 42);
    assert((arg1 - 1)(43) == 42);
    assert((arg1 * 2)(21) == 42);
    assert((arg1 / 2)(84) == 42);
    assert((arg1 % 5)(47) == 2);
    std::cout << "demo 01 ok\n";
}
