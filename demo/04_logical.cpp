// demo 04: logical operators
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((arg1 && true)(true));
    assert(!(arg1 && true)(false));
    assert((arg1 || false)(true));
    assert(!(arg1 || false)(false));
    assert((!arg1)(0));
    assert((!arg1)(1) == false);
    std::cout << "demo 04 ok\n";
}
