// demo 18: the last placeholder, arg29
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int twice(int x) { return x * 2; }

int main() {
    int r1 = (arg1 + arg29)(
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29);
    assert(r1 == 30);

    int r2 = arg29(21)(
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, twice);
    assert(r2 == 42);

    std::cout << "demo 18 ok\n";
}
