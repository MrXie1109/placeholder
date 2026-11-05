// demo 15: use with STL algorithms
#include "../placeholder.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace placeholder;

int main() {
    std::vector<int> src{1, 2, 3}, dst;
    std::transform(src.begin(), src.end(), std::back_inserter(dst), arg1 * 2);
    assert((dst == std::vector<int>{2, 4, 6}));

    std::vector<int> dst2;
    std::transform(src.begin(), src.end(), std::back_inserter(dst2), (arg1 + 1) * 10);
    assert((dst2 == std::vector<int>{20, 30, 40}));

    std::cout << "demo 15 ok\n";
}
