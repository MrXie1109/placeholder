// demo 10: subscript
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace placeholder;

int main() {
    std::vector<int> v{7, 8, 9};
    assert((arg1[0])(v) == 7);
    assert((arg1[1])(v) == 8);
    assert((arg1[2])(v) == 9);

    int arr[3] = {1, 2, 3};
    assert(((arg1 + 1)[0])(arr) == 2);

    std::cout << "demo 10 ok\n";
}
