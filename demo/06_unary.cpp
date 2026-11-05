// demo 06: unary operators
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((-arg1)(-42) == 42);
    assert((+arg1)(42) == 42);
    assert((!arg1)(0));
    assert((~arg1)(-43) == 42);

    int n = 42;
    assert((*arg1)(&n) == 42);
    std::cout << "demo 06 ok\n";
}
