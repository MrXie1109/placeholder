// demo 09: assignment node
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    int x = 0;
    (arg1 = 42)(x);
    assert(x == 42);

    int y = 0;
    (arg1 = arg2)(y, 7);
    assert(y == 7);

    std::cout << "demo 09 ok\n";
}
