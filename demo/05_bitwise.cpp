// demo 05: bitwise operators
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((arg1 & 0xFF)(0x1FF) == 0xFF);
    assert((arg1 | 1)(40) == 41);
    assert((arg1 ^ 1)(43) == 42);
    assert((arg1 << 1)(21) == 42);
    assert((arg1 >> 1)(84) == 42);
    assert((1 << arg1)(5) == 32);
    std::cout << "demo 05 ok\n";
}
