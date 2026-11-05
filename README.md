# placeholder.hpp

[中文](README.zh.md)

Header-only placeholder expressions for C++11. `arg1` … `arg29` stand for the
1st … 29th argument of a later call. Operators applied to a placeholder build a
lightweight expression tree, and the tree is evaluated only when it is invoked
with the real arguments. Everything lives in `namespace placeholder`.

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

## Requirements

- C++11 or later
- Standard library only: `<cstddef>`, `<tuple>`, `<type_traits>`, `<utility>`

## Quick start

```cpp
#include "placeholder.hpp"
#include <cassert>

using namespace placeholder;

int twice(int x) { return x * 2; }

int main() {
    // Binary, placeholder on either side
    assert((arg1 + 1)(41) == 42);
    assert((1 + arg1)(41) == 42);
    assert((100 - arg1)(58) == 42);
    assert((arg1 * 2)(21) == 42);
    assert((arg1 < 42)(41));

    // Multiple placeholders
    assert((arg1 + arg2)(3, 4) == 7);
    assert((arg1 * arg2 - arg3)(5, 6, 7) == 23);

    // Deferred function call
    assert(arg1(42)(twice) == 84);
    assert(arg1(6, 7)([](int a, int b) { return a * b; }) == 42);

    // Nested expressions
    assert(((arg1 + 1) * 2)(20) == 42);
    assert(((arg1 < 10) && (arg1 > 0))(5));
}
```

## Placeholders

| Object  | Argument index |
| ------- | -------------- |
| `arg1`  | 0              |
| `arg2`  | 1              |
| …       | …              |
| `arg29` | 28             |

Placeholders are `static constexpr` objects with internal linkage inside
`namespace placeholder`, so the header can be included from multiple translation
units without ODR issues.

## Operators

Every operator below builds an expression node; nothing is evaluated until the
node is called.

### Binary

`+` `-` `*` `/` `%` `==` `!=` `<` `<=` `>` `>=` `&&` `||` `&` `|` `^` `<<` `>>`

Both operand orders are supported:

```cpp
(arg1 + 5)(41);   // 46
(5 + arg1)(41);   // 46
(arg1 + arg2)(3, 4); // 7
```

### Unary

Prefix: `+` `-` `!` `~` `*` `++` `--`
Postfix: `++` `--`

```cpp
(-arg1)(-42);     // 42
(!arg1)(0);       // true
(*arg1)(&n);      // n
(++arg1)(x);      // ++x
(arg1++)(x);      // x++
```

### Assignment and compound assignment

These must be members in C++, so they are provided by the placeholder itself:

```cpp
(arg1 = 42)(x);        // x = 42
(arg1 += 2)(x);        // x += 2
(arg1 <<= 1)(x);       // x <<= 1
```

### Subscript

```cpp
(arg1[0])(v);          // v[0]
((arg1 + 1)[0])(p);    // p[1]
```

### Function call

`arg1(args...)` stores the arguments and evaluates to `f(args...)`, where `f` is
the value of the placeholder:

```cpp
arg1(42)(foo);         // foo(42)
arg1(1, 2)(plus);      // plus(1, 2)
arg2(42)(0, foo);      // foo(42)
```

## Expression nodes

| Node                  | Meaning                        |
| --------------------- | ------------------------------ |
| `_binary<L, R, Op>`   | `l op r`                       |
| `_unary<X, Op>`       | `op x` or `x op`               |
| `_call<F, A...>`      | `f(a...)`                      |

Nodes are values: they store their operands by value (after `std::decay`), can
be copied, and can be reused with different arguments. Every node provides both
`const` and non-const `operator()`.

```cpp
auto f = (arg1 + 1) * 2;
f(20);                  // 42
const auto g = arg1 + arg2;
g(1, 2);                // 3
```

## Evaluation model

- Building an expression performs no computation, only node construction.
- Calling the expression dispatches every leaf through `_eval`:
  - a placeholder selects its argument by index;
  - a nested node is evaluated recursively with the same argument pack;
  - a plain value is passed through unchanged.
- Arguments are accessed as lvalues, so a single argument can be read by
  multiple leaves without being moved.

## Limitations

- `&&` and `||` are overloaded and therefore do **not** short-circuit.
- Arguments are passed as lvalues; rvalue-only use inside an expression is not
  supported.
- Stored operands and call arguments are kept by value after decay.
- Arguments of a call node (`arg1(42)`) are passed through as-is; they are not
  themselves evaluated as placeholder expressions.
- Operators that cannot be overloaded (`?:`, `sizeof`, `::`, `.*`, `->*`) are
  not supported.
- The free operators are enabled only when at least one operand is a
  placeholder or an expression node, detected via the
  `_placeholder_arg` / `_placeholder_expr` member aliases. Ordinary user types
  are untouched.

## Compatibility

```bash
g++ -std=c++11 -Wall -Wextra -pedantic-errors -Werror demo/01_basic_arithmetic.cpp -o demo01
./demo01
```

Every file in `demo/` can be compiled the same way.

The implementation uses only C++11 features: trailing return types, alias
templates, variadic templates, `std::tuple`, `std::enable_if` and
`std::integral_constant`.

## Files

- `placeholder.hpp` — the library, all declarations in `namespace placeholder`
- `demo/` — 20 runnable examples, one per topic
- `README.md`, `README.zh.md` — documentation
