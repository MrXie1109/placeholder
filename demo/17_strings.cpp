// demo 17: strings and non-trivial types
#include "../placeholder.hpp"
#include <cassert>
#include <iostream>
#include <string>

using namespace placeholder;

int main() {
    std::string a = "hello", b = " world";
    assert((arg1 + b)(a) == "hello world");
    assert((a + arg1)(b) == "hello world");
    assert((arg1 + arg1)(std::string("ab")) == "abab");

    std::cout << "demo 17 ok\n";
}
