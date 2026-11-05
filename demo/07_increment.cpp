// demo 07: prefix and postfix increment/decrement
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    int x = 41;
    assert((++arg1)(x) == 42 && x == 42);

    x = 41;
    assert((arg1++)(x) == 41 && x == 42);

    x = 43;
    assert((--arg1)(x) == 42 && x == 42);

    x = 43;
    assert((arg1--)(x) == 43 && x == 42);

    std::cout << "demo 07 ok\n";
}
