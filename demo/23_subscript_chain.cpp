// demo 23: subscript after dereference and after a call
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

int *make(int x) {
    static int a[3];
    a[0] = x;
    return a;
}

int main() {
    int arr[3] = {7, 8, 9};
    int *p = arr;
    int **pp = &p;

    assert(((*arg1)[0])(pp) == 7);
    assert(((*arg1)[2])(pp) == 9);

    assert((arg1(42)[0])(make) == 42);

    std::cout << "demo 23 ok\n";
}
