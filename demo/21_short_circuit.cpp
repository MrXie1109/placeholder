// demo 21: short-circuit evaluation of && and ||
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    int calls = 0;
    auto side = [&](int v) { ++calls; return v; };

    auto land = arg1 && arg2(0);
    assert(land(false, side) == false);
    assert(calls == 0);                  // right side skipped
    assert(land(true, side) == false);   // side(0) returns 0
    assert(calls == 1);

    auto lor = arg1 || arg2(1);
    assert(lor(true, side) == true);
    assert(calls == 1);                  // right side skipped
    assert(lor(false, side) == true);    // side(1) returns 1
    assert(calls == 2);

    std::cout << "demo 21 ok\n";
}
