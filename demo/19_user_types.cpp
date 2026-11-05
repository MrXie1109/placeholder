// demo 19: user-defined types
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

struct Point {
    int x, y;
};

Point operator+(Point a, Point b) { return Point{a.x + b.x, a.y + b.y}; }
bool operator==(Point a, Point b) { return a.x == b.x && a.y == b.y; }

int main() {
    Point a{1, 2}, b{3, 4};

    Point sum = (arg1 + arg2)(a, b);
    assert((sum == Point{4, 6}));

    Point sum2 = (a + arg1)(b);
    assert((sum2 == Point{4, 6}));

    std::cout << "demo 19 ok\n";
}
