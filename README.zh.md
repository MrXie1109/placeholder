# placeholder.hpp

[English](README.md)

C++11 的 header-only 占位符表达式库。`arg1` … `arg29` 分别代表后续调用时的第
1 … 29 个参数；作用在占位符上的运算符会构建一棵轻量表达式树，只有用真实参数
调用时才会求值。所有内容位于 `namespace placeholder` 中。

```cpp
#include "placeholder.hpp"

using namespace placeholder;

int twice(int x) { return x * 2; }

int main() {
    (arg1 + 1)(41);          // 42
    (1 + arg1)(41);          // 42
    (arg1 + arg2)(3, 4);     // 7
    arg1(42)(twice);         // twice(42) == 84
    ((arg1 + 1) * 2)(20);    // 42
}
```

## 环境要求

- C++11 或更高版本
- 只依赖标准库：`<cstddef>`、`<tuple>`、`<type_traits>`、`<utility>`

## 快速开始

```cpp
#include "placeholder.hpp"
#include <cassert>

using namespace placeholder;

int twice(int x) { return x * 2; }

int main() {
    // 二元：占位符在左或在右
    assert((arg1 + 1)(41) == 42);
    assert((1 + arg1)(41) == 42);
    assert((100 - arg1)(58) == 42);
    assert((arg1 * 2)(21) == 42);
    assert((arg1 < 42)(41));

    // 多占位符
    assert((arg1 + arg2)(3, 4) == 7);
    assert((arg1 * arg2 - arg3)(5, 6, 7) == 23);

    // 延迟函数调用
    assert(arg1(42)(twice) == 84);
    assert(arg1(6, 7)([](int a, int b) { return a * b; }) == 42);

    // 嵌套表达式
    assert(((arg1 + 1) * 2)(20) == 42);
    assert(((arg1 < 10) && (arg1 > 0))(5));
}
```

## 与 lambda 对比

操作本身是纯表达式时，占位符写法更短：没有参数列表、没有返回类型、没有函数体。

| 操作 | lambda | placeholder.hpp |
| --- | --- | --- |
| 加一 | `[](int x) { return x + 1; }` | `arg1 + 1` |
| 求和 | `[](int a, int b) { return a + b; }` | `arg1 + arg2` |
| 区间判断 | `[](int x) { return x > 0 && x < 10; }` | `(arg1 > 0) && (arg1 < 10)` |
| 解引用后下标 | `[](int **p) { return (*p)[0]; }` | `(*arg1)[0]` |
| 成员访问 | `[](Point *p) { return p->x; }` | `arg1->*&Point::x` |

操作是单个表达式时用占位符最合适；需要语句、局部变量或控制流时用 lambda。

## 占位符

| 对象    | 参数下标 |
| ------- | -------- |
| `arg1`  | 0        |
| `arg2`  | 1        |
| …       | …        |
| `arg29` | 28       |

占位符是 `namespace placeholder` 内部链接的 `static constexpr` 对象，头文件可以
被多个翻译单元同时包含，不会有 ODR 问题。

## 运算符

下面的运算符都只构建表达式节点，不会立即求值；只有调用节点时才计算结果。

### 二元

`+` `-` `*` `/` `%` `==` `!=` `<` `<=` `>` `>=` `&&` `||` `&` `|` `^` `<<` `>>` `->*`

左右两种顺序都支持：

```cpp
(arg1 + 5)(41);      // 46
(5 + arg1)(41);      // 46
(arg1 + arg2)(3, 4); // 7
```

### 一元

前缀：`+` `-` `!` `~` `*` `++` `--`
后缀：`++` `--`

```cpp
(-arg1)(-42);     // 42
(!arg1)(0);       // true
(*arg1)(&n);      // n
(++arg1)(x);      // ++x
(arg1++)(x);      // x++
```

### 赋值与复合赋值

C++ 规定这些运算符必须是成员函数，因此由占位符自身提供：

```cpp
(arg1 = 42)(x);     // x = 42
(arg1 += 2)(x);     // x += 2
(arg1 <<= 1)(x);    // x <<= 1
```

### 短路求值

`&&` 和 `||` 不走 `Op::apply`，而是由专门的节点先求左操作数，只在需要时才求右
操作数：

```cpp
int calls = 0;
auto side = [&](int v) { ++calls; return v; };
auto land = arg1 && arg2(0);

land(false, side);   // false，calls == 0
land(true, side);    // false，calls == 1
```

### 下标

```cpp
(arg1[0])(v);       // v[0]
((arg1 + 1)[0])(p); // p[1]
```

下标同样可以接在派生节点后面：

```cpp
((*arg1)[0])(pp);      // (*pp)[0]
(arg1(42)[0])(make);   // make(42)[0]
```

### 成员指针

C++ 里 `.*` 不能重载，成员指针请用 `->*`：

```cpp
struct Point { int x; };
int Point::*px = &Point::x;
Point p{42};

(arg1->*px)(&p);       // 42
((arg1->*px) + 1)(&p); // 43
```

### 函数调用

`arg1(args...)` 保存实参，求值结果为 `f(args...)`，其中 `f` 是占位符取到的值：

```cpp
arg1(42)(foo);      // foo(42)
arg1(1, 2)(plus);   // plus(1, 2)
arg2(42)(0, foo);   // foo(42)
```

## 表达式节点

| 节点                 | 含义              |
| -------------------- | ----------------- |
| `_binary<L, R, Op>`  | `l op r`          |
| `_unary<X, Op>`      | `op x` 或 `x op`  |
| `_call<F, A...>`     | `f(a...)`         |

节点是值类型：操作数按 `std::decay` 后的类型按值保存，节点可复制、可在不同参数
下反复使用。每个节点都同时提供 `const` 和 non-const 的 `operator()`。

```cpp
auto f = (arg1 + 1) * 2;
f(20);                  // 42
const auto g = arg1 + arg2;
g(1, 2);                // 3
```

## 求值模型

- 构建表达式阶段不做任何计算，只构造节点。
- 调用表达式时，每个叶子通过 `_eval` 分派：
  - 占位符按索引取出对应实参；
  - 嵌套节点用同一个参数包递归求值；
  - 普通值原样传递。
- 所有叶子以左值方式读取实参，因此同一个实参可以被多个叶子读取而不会被移动。

## 限制

- `&&` 和 `||` 支持短路；两侧操作数都需要能转换为 `bool`。
- `.*` 不能重载，成员指针只支持 `->*`。
- 实参以左值传递；表达式内部不支持只接受右值的用法。
- 保存的操作数和调用实参都按 decay 后的类型按值存储。
- 调用节点 `arg1(42)` 的实参原样传递，不会再把它们当作占位符表达式求值。
- 无法重载的运算符（`?:`、`sizeof`、`::`、`.*`、`->*`）不支持。
- 自由运算符只在至少一侧是占位符或表达式节点时启用，通过
  `_placeholder_arg` / `_placeholder_expr` 成员别名做 SFINAE 检测，不会误抓
  普通用户类型。

## 兼容性

```bash
g++ -std=c++11 -Wall -Wextra -pedantic-errors -Werror demo/01_basic_arithmetic.cpp -o demo01
./demo01
```

`demo/` 下的每个文件都可以用同样方式编译。

实现只用到 C++11 特性：尾置返回类型、alias template、可变参数模板、
`std::tuple`、`std::enable_if` 和 `std::integral_constant`。

## 文件

- `placeholder.hpp` — 库本体，所有声明位于 `namespace placeholder`
- `demo/` — 23 个可运行示例，每个对应一个主题
- `README.md`、`README.zh.md` — 文档
