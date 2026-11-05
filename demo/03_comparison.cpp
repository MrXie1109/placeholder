// demo 03: comparison operators
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int main() {
    assert((arg1 < 42)(41));
    assert((arg1 <= 42)(42));
    assert((arg1 > 42)(43));
    assert((arg1 >= 42)(42));
    assert((arg1 == 42)(42));
    assert((arg1 != 42)(41));
    assert((42 < arg1)(43));
    std::cout << "demo 03 ok\n";
}
