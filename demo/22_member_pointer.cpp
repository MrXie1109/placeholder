// demo 22: member pointer access through arg1->*pm
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>

using namespace placeholder;

struct Point {
    int x;
    int y;
};

int main() {
    int Point::*px = &Point::x;

    Point p{42, 7};
    Point *pp = &p;

    assert((arg1->*px)(pp) == 42);
    assert(((arg1->*px) + 1)(pp) == 43);

    int Point::*py = &Point::y;
    assert((arg1->*py)(pp) == 7);

    std::cout << "demo 22 ok\n";
}
