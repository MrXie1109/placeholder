/**
 * @file placeholder.hpp
 * @brief Deferred placeholder expressions for C++11.
 *
 * The objects @c arg1 through @c arg29 stand for the first through
 * twenty-ninth argument of a later call. Operators applied to a placeholder build a
 * lightweight expression tree, which is evaluated when the tree is invoked
 * with the actual arguments:
 *
 * @code
 * (arg1 + 1)(41);          // 42
 * (arg1 + arg2)(3, 4);     // 7
 * (arg1 < arg2)(1, 2);     // true
 * arg1(42)(foo);           // foo(42)
 * @endcode
 *
 * Every expression node provides both const and non-const @c operator().
 */

#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace placeholder {

/**
 * @brief Applies @c std::decay to @p T.
 * @tparam T Type to decay.
 */
template <class T>
using _d = typename std::decay<T>::type;

/**
 * @brief Declares a placeholder tag.
 * @param n Placeholder number.
 */
#define PLACEHOLDER_ARG_DECL(n) struct _arg##n;

/**
 * @name Placeholder tags
 * @{
 */
PLACEHOLDER_ARG_DECL(1)
PLACEHOLDER_ARG_DECL(2)
PLACEHOLDER_ARG_DECL(3)
PLACEHOLDER_ARG_DECL(4)
PLACEHOLDER_ARG_DECL(5)
PLACEHOLDER_ARG_DECL(6)
PLACEHOLDER_ARG_DECL(7)
PLACEHOLDER_ARG_DECL(8)
PLACEHOLDER_ARG_DECL(9)
PLACEHOLDER_ARG_DECL(10)
PLACEHOLDER_ARG_DECL(11)
PLACEHOLDER_ARG_DECL(12)
PLACEHOLDER_ARG_DECL(13)
PLACEHOLDER_ARG_DECL(14)
PLACEHOLDER_ARG_DECL(15)
PLACEHOLDER_ARG_DECL(16)
PLACEHOLDER_ARG_DECL(17)
PLACEHOLDER_ARG_DECL(18)
PLACEHOLDER_ARG_DECL(19)
PLACEHOLDER_ARG_DECL(20)
PLACEHOLDER_ARG_DECL(21)
PLACEHOLDER_ARG_DECL(22)
PLACEHOLDER_ARG_DECL(23)
PLACEHOLDER_ARG_DECL(24)
PLACEHOLDER_ARG_DECL(25)
PLACEHOLDER_ARG_DECL(26)
PLACEHOLDER_ARG_DECL(27)
PLACEHOLDER_ARG_DECL(28)
PLACEHOLDER_ARG_DECL(29)
/** @} */

/**
 * @name Expression node templates
 * @{
 */
template <class L, class R, class Op> struct _binary;
template <class X, class Op> struct _unary;
template <class F, class... A> struct _call;
/** @} */

/**
 * @brief Maps a placeholder tag to its zero-based argument index.
 * @tparam T Placeholder tag.
 */
template <class T> struct _arg_index;

/**
 * @brief Specializes @c _arg_index for one placeholder.
 * @param n Placeholder number.
 * @param i Zero-based argument index.
 */
#define PLACEHOLDER_ARG_INDEX(n, i) \
    template <> struct _arg_index<_arg##n> : std::integral_constant<std::size_t, i> {};

PLACEHOLDER_ARG_INDEX(1, 0)
PLACEHOLDER_ARG_INDEX(2, 1)
PLACEHOLDER_ARG_INDEX(3, 2)
PLACEHOLDER_ARG_INDEX(4, 3)
PLACEHOLDER_ARG_INDEX(5, 4)
PLACEHOLDER_ARG_INDEX(6, 5)
PLACEHOLDER_ARG_INDEX(7, 6)
PLACEHOLDER_ARG_INDEX(8, 7)
PLACEHOLDER_ARG_INDEX(9, 8)
PLACEHOLDER_ARG_INDEX(10, 9)
PLACEHOLDER_ARG_INDEX(11, 10)
PLACEHOLDER_ARG_INDEX(12, 11)
PLACEHOLDER_ARG_INDEX(13, 12)
PLACEHOLDER_ARG_INDEX(14, 13)
PLACEHOLDER_ARG_INDEX(15, 14)
PLACEHOLDER_ARG_INDEX(16, 15)
PLACEHOLDER_ARG_INDEX(17, 16)
PLACEHOLDER_ARG_INDEX(18, 17)
PLACEHOLDER_ARG_INDEX(19, 18)
PLACEHOLDER_ARG_INDEX(20, 19)
PLACEHOLDER_ARG_INDEX(21, 20)
PLACEHOLDER_ARG_INDEX(22, 21)
PLACEHOLDER_ARG_INDEX(23, 22)
PLACEHOLDER_ARG_INDEX(24, 23)
PLACEHOLDER_ARG_INDEX(25, 24)
PLACEHOLDER_ARG_INDEX(26, 25)
PLACEHOLDER_ARG_INDEX(27, 26)
PLACEHOLDER_ARG_INDEX(28, 27)
PLACEHOLDER_ARG_INDEX(29, 28)

/**
 * @brief Selects the @p I -th element of a parameter pack.
 *
 * Arguments are taken as lvalues, so the same argument can be read more than
 * once without being moved.
 *
 * @tparam I Zero-based index.
 */
template <std::size_t I>
struct _at {
    template <class T, class... Ts>
    static auto get(T &, Ts &... ts) -> decltype(_at<I - 1>::get(ts...)) {
        return _at<I - 1>::get(ts...);
    }
};

template <>
struct _at<0> {
    template <class T, class... Ts>
    static T &get(T &t, Ts &...) { return t; }
};

/**
 * @brief Minimal C++11 index sequence.
 * @tparam Is Index values.
 */
template <std::size_t... Is>
struct _index_seq {};

/**
 * @brief Builds @c _index_seq<0, ..., N-1>.
 * @tparam N Number of indices.
 * @tparam Is Accumulated indices (internal).
 */
template <std::size_t N, std::size_t... Is>
struct _make_index_seq
    : _make_index_seq<N - 1, N - 1, Is...> {
};

template <std::size_t... Is>
struct _make_index_seq<0, Is...> {
    using type = _index_seq<Is...>;
};

/**
 * @brief Alias for @c _make_index_seq<N>::type.
 * @tparam N Number of indices.
 */
template <std::size_t N>
using _index_seq_t = typename _make_index_seq<N>::type;

/**
 * @brief Invokes @p fn with every element of @p t.
 * @tparam Fn Callable type.
 * @tparam Tuple Tuple type.
 * @tparam Is Index values.
 * @param fn Callable object.
 * @param t Argument tuple.
 * @return Result of @c fn(std::get<Is>(t)...).
 */
template <class Fn, class Tuple, std::size_t... Is>
auto _apply_tuple(Fn &&fn, const Tuple &t, _index_seq<Is...>)
    -> decltype(std::forward<Fn>(fn)(std::get<Is>(t)...)) {
    return std::forward<Fn>(fn)(std::get<Is>(t)...);
}

/**
 * @brief Detects placeholder tags.
 *
 * A placeholder declares @c using _placeholder_arg = void; Detection goes
 * through that member alias and never instantiates unrelated user types.
 *
 * @tparam T Candidate type.
 */
template <class T, class = void>
struct _is_arg
    : std::false_type {
};

template <class T>
struct _is_arg<T, typename std::enable_if<
    std::is_same<typename _d<T>::_placeholder_arg, void>::value>::type>
    : std::true_type {
};

/**
 * @brief Detects expression nodes.
 *
 * Every node declares @c using _placeholder_expr = void;
 *
 * @tparam T Candidate type.
 */
template <class T, class = void>
struct _is_expr
    : std::false_type {
};

template <class T>
struct _is_expr<T, typename std::enable_if<
    std::is_same<typename _d<T>::_placeholder_expr, void>::value>::type>
    : std::true_type {
};

/**
 * @brief True when @p T is a placeholder or an expression node.
 * @tparam T Candidate type.
 */
template <class T>
struct _is_operand
    : std::integral_constant<bool,
          _is_arg<T>::value || _is_expr<T>::value> {
};

/**
 * @brief True when @p T can be stored by value after decay.
 * @tparam T Candidate type.
 */
template <class T>
struct _bindable
    : std::is_constructible<_d<T>, T &&> {
};

/**
 * @brief Defines a tag type for a binary operator.
 * @param name Tag name; the generated type is @c _op_ followed by @p name.
 * @param op Operator token.
 */
#define PLACEHOLDER_TAG2(name, op)                                          \
    struct _op_##name {                                                     \
        template <class A, class B>                                         \
        static auto apply(A &&a, B &&b)                                     \
            -> decltype(std::forward<A>(a) op std::forward<B>(b)) {         \
            return std::forward<A>(a) op std::forward<B>(b);                \
        }                                                                   \
    };

/**
 * @brief Defines a tag type for a prefix unary operator.
 * @param name Tag name; the generated type is @c _op_ followed by @p name.
 * @param op Operator token.
 */
#define PLACEHOLDER_TAG1(name, op)                                          \
    struct _op_##name {                                                     \
        template <class A>                                                  \
        static auto apply(A &&a) -> decltype(op std::forward<A>(a)) {       \
            return op std::forward<A>(a);                                   \
        }                                                                   \
    };

/**
 * @brief Defines a tag type for a postfix unary operator.
 * @param name Tag name; the generated type is @c _op_ followed by @p name.
 * @param op Operator token.
 */
#define PLACEHOLDER_TAGPOST(name, op)                                       \
    struct _op_##name {                                                     \
        template <class A>                                                  \
        static auto apply(A &&a) -> decltype(std::forward<A>(a) op) {       \
            return std::forward<A>(a) op;                                   \
        }                                                                   \
    };

PLACEHOLDER_TAG2(add, +)
PLACEHOLDER_TAG2(sub, -)
PLACEHOLDER_TAG2(mul, *)
PLACEHOLDER_TAG2(div, /)
PLACEHOLDER_TAG2(mod, %)
PLACEHOLDER_TAG2(eq, ==)
PLACEHOLDER_TAG2(ne, !=)
PLACEHOLDER_TAG2(lt, <)
PLACEHOLDER_TAG2(le, <=)
PLACEHOLDER_TAG2(gt, >)
PLACEHOLDER_TAG2(ge, >=)
PLACEHOLDER_TAG2(land, &&)
PLACEHOLDER_TAG2(lor, ||)
PLACEHOLDER_TAG2(band, &)
PLACEHOLDER_TAG2(bor, |)
PLACEHOLDER_TAG2(bxor, ^)
PLACEHOLDER_TAG2(shl, <<)
PLACEHOLDER_TAG2(shr, >>)

PLACEHOLDER_TAG2(assign, =)
PLACEHOLDER_TAG2(add_assign, +=)
PLACEHOLDER_TAG2(sub_assign, -=)
PLACEHOLDER_TAG2(mul_assign, *=)
PLACEHOLDER_TAG2(div_assign, /=)
PLACEHOLDER_TAG2(mod_assign, %=)
PLACEHOLDER_TAG2(band_assign, &=)
PLACEHOLDER_TAG2(bor_assign, |=)
PLACEHOLDER_TAG2(bxor_assign, ^=)
PLACEHOLDER_TAG2(shl_assign, <<=)
PLACEHOLDER_TAG2(shr_assign, >>=)

PLACEHOLDER_TAG2(arrowstar, ->*)

PLACEHOLDER_TAG1(neg, -)
PLACEHOLDER_TAG1(pos, +)
PLACEHOLDER_TAG1(lnot, !)
PLACEHOLDER_TAG1(bnot, ~)
PLACEHOLDER_TAG1(deref, *)
PLACEHOLDER_TAG1(preinc, ++)
PLACEHOLDER_TAG1(predec, --)

PLACEHOLDER_TAGPOST(postinc, ++)
PLACEHOLDER_TAGPOST(postdec, --)

/**
 * @brief Tag for the subscript operator.
 *
 * Declared separately because @c a[b] does not fit the @c a\ op\ b macro form.
 */
struct _op_subscript {
    template <class A, class B>
    static auto apply(A &&a, B &&b)
        -> decltype(std::forward<A>(a)[std::forward<B>(b)]) {
        return std::forward<A>(a)[std::forward<B>(b)];
    }
};

/**
 * @brief Evaluates a single operand of an expression node.
 *
 * The specializations dispatch on @p IsExpr and @p IsArg:
 * - expression node: recursively evaluate with the same argument pack;
 * - placeholder: select the argument at the tag's position;
 * - plain value: return the stored value unchanged.
 *
 * @tparam T Operand type (possibly cv/ref qualified).
 * @tparam IsExpr Whether @p T is an expression node.
 * @tparam IsArg Whether @p T is a placeholder tag.
 * @tparam Args Argument pack types.
 */
template <class T, bool IsExpr, bool IsArg, class... Args>
struct _eval_impl;

template <class T, class... Args>
struct _eval_impl<T, true, false, Args...> {
    static auto get(T &&t, Args &... args)
        -> decltype(static_cast<T &&>(t)(args...)) {
        return static_cast<T &&>(t)(args...);
    }
};

template <class T, class... Args>
struct _eval_impl<T, false, true, Args...> {
    static auto get(T &&, Args &... args)
        -> decltype(_at<_arg_index<_d<T>>::value>::get(args...)) {
        return _at<_arg_index<_d<T>>::value>::get(args...);
    }
};

template <class T, class... Args>
struct _eval_impl<T, false, false, Args...> {
    static T &&get(T &&t, Args &...) { return static_cast<T &&>(t); }
};

/**
 * @brief Evaluates an operand with the supplied argument pack.
 * @tparam T Operand type.
 * @tparam Args Argument pack types.
 */
template <class T, class... Args>
struct _eval
    : _eval_impl<T, _is_expr<T>::value, _is_arg<T>::value, Args...> {
};

/**
 * @brief Deferred function call.
 *
 * @c arg1(42) stores @c 42 and evaluates to a call of the first argument:
 * @code
 * arg1(42)(foo);    // foo(42)
 * arg1(1, 2)(f);    // f(1, 2)
 * @endcode
 *
 * @tparam F Callee expression.
 * @tparam A Stored argument types.
 */
template <class F, class... A>
struct _call {
    using _placeholder_expr = void;

    F f;
    std::tuple<A...> a;

    _call(F f_, const std::tuple<A...> &a_)
        : f(static_cast<F &&>(f_)), a(a_) {}

    template <class... Args>
    auto operator()(Args &&... args)
        -> decltype(static_cast<const _call &>(*this)(
            std::forward<Args>(args)...)) {
        return static_cast<const _call &>(*this)(
            std::forward<Args>(args)...);
    }

    template <class... Args>
    auto operator()(Args &&... args) const
        -> decltype(_apply_tuple(_eval<const F &, Args...>::get(f, args...), a,
                                 _index_seq_t<sizeof...(A)>())) {
        return _apply_tuple(_eval<const F &, Args...>::get(f, args...), a,
                            _index_seq_t<sizeof...(A)>());
    }

    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<_call, _d<T>, _op_subscript> operator[](T &&t) const {
        return {*this, std::forward<T>(t)};
    }
};

/**
 * @brief Functionality shared by all placeholders.
 *
 * Assignment, compound assignment, subscript and call operators must be
 * non-static members in C++, so they are declared here.
 *
 * @tparam Derived Concrete placeholder tag.
 */
template <class Derived>
struct _arg_base {
    using _placeholder_arg = void;

/**
 * @brief Declares a member operator that builds a binary node.
 * @param name Operator tag name.
 * @param op Operator token.
 */
#define PLACEHOLDER_ARG_MEMBER(name, op)                                    \
    template <class T,                                                      \
              class = typename std::enable_if<                             \
                  _bindable<T>::value>::type>                               \
    _binary<Derived, _d<T>, _op_##name> operator op(T &&t) const {          \
        return {Derived(), std::forward<T>(t)};                             \
    }

    PLACEHOLDER_ARG_MEMBER(assign, =)
    PLACEHOLDER_ARG_MEMBER(add_assign, +=)
    PLACEHOLDER_ARG_MEMBER(sub_assign, -=)
    PLACEHOLDER_ARG_MEMBER(mul_assign, *=)
    PLACEHOLDER_ARG_MEMBER(div_assign, /=)
    PLACEHOLDER_ARG_MEMBER(mod_assign, %=)
    PLACEHOLDER_ARG_MEMBER(band_assign, &=)
    PLACEHOLDER_ARG_MEMBER(bor_assign, |=)
    PLACEHOLDER_ARG_MEMBER(bxor_assign, ^=)
    PLACEHOLDER_ARG_MEMBER(shl_assign, <<=)
    PLACEHOLDER_ARG_MEMBER(shr_assign, >>=)

    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<Derived, _d<T>, _op_subscript> operator[](T &&t) const {
        return {Derived(), std::forward<T>(t)};
    }

    /**
     * @brief Builds a deferred call of this placeholder.
     * @tparam A Stored argument types.
     * @param a Arguments to pass to the placeholder value.
     * @return A @c _call node.
     */
    template <class... A>
    _call<Derived, _d<A>...> operator()(A &&... a) {
        return {Derived(), std::tuple<_d<A>...>(std::forward<A>(a)...)};
    }

    template <class... A>
    _call<Derived, _d<A>...> operator()(A &&... a) const {
        return {Derived(), std::tuple<_d<A>...>(std::forward<A>(a)...)};
    }
};

/**
 * @brief Defines one placeholder tag.
 * @param n Placeholder number.
 */
#define PLACEHOLDER_ARG_TAG(n) \
    struct _arg##n : _arg_base<_arg##n> { \
        using _arg_base<_arg##n>::operator=; \
    };

PLACEHOLDER_ARG_TAG(1)
PLACEHOLDER_ARG_TAG(2)
PLACEHOLDER_ARG_TAG(3)
PLACEHOLDER_ARG_TAG(4)
PLACEHOLDER_ARG_TAG(5)
PLACEHOLDER_ARG_TAG(6)
PLACEHOLDER_ARG_TAG(7)
PLACEHOLDER_ARG_TAG(8)
PLACEHOLDER_ARG_TAG(9)
PLACEHOLDER_ARG_TAG(10)
PLACEHOLDER_ARG_TAG(11)
PLACEHOLDER_ARG_TAG(12)
PLACEHOLDER_ARG_TAG(13)
PLACEHOLDER_ARG_TAG(14)
PLACEHOLDER_ARG_TAG(15)
PLACEHOLDER_ARG_TAG(16)
PLACEHOLDER_ARG_TAG(17)
PLACEHOLDER_ARG_TAG(18)
PLACEHOLDER_ARG_TAG(19)
PLACEHOLDER_ARG_TAG(20)
PLACEHOLDER_ARG_TAG(21)
PLACEHOLDER_ARG_TAG(22)
PLACEHOLDER_ARG_TAG(23)
PLACEHOLDER_ARG_TAG(24)
PLACEHOLDER_ARG_TAG(25)
PLACEHOLDER_ARG_TAG(26)
PLACEHOLDER_ARG_TAG(27)
PLACEHOLDER_ARG_TAG(28)
PLACEHOLDER_ARG_TAG(29)

/**
 * @brief Defines one placeholder object.
 * @param n Placeholder number.
 */
#define PLACEHOLDER_ARG_OBJECT(n) static constexpr _arg##n arg##n{};

/**
 * @name Placeholder objects
 * @{
 */
PLACEHOLDER_ARG_OBJECT(1)
PLACEHOLDER_ARG_OBJECT(2)
PLACEHOLDER_ARG_OBJECT(3)
PLACEHOLDER_ARG_OBJECT(4)
PLACEHOLDER_ARG_OBJECT(5)
PLACEHOLDER_ARG_OBJECT(6)
PLACEHOLDER_ARG_OBJECT(7)
PLACEHOLDER_ARG_OBJECT(8)
PLACEHOLDER_ARG_OBJECT(9)
PLACEHOLDER_ARG_OBJECT(10)
PLACEHOLDER_ARG_OBJECT(11)
PLACEHOLDER_ARG_OBJECT(12)
PLACEHOLDER_ARG_OBJECT(13)
PLACEHOLDER_ARG_OBJECT(14)
PLACEHOLDER_ARG_OBJECT(15)
PLACEHOLDER_ARG_OBJECT(16)
PLACEHOLDER_ARG_OBJECT(17)
PLACEHOLDER_ARG_OBJECT(18)
PLACEHOLDER_ARG_OBJECT(19)
PLACEHOLDER_ARG_OBJECT(20)
PLACEHOLDER_ARG_OBJECT(21)
PLACEHOLDER_ARG_OBJECT(22)
PLACEHOLDER_ARG_OBJECT(23)
PLACEHOLDER_ARG_OBJECT(24)
PLACEHOLDER_ARG_OBJECT(25)
PLACEHOLDER_ARG_OBJECT(26)
PLACEHOLDER_ARG_OBJECT(27)
PLACEHOLDER_ARG_OBJECT(28)
PLACEHOLDER_ARG_OBJECT(29)
/** @} */

/**
 * @brief Binary expression node representing @c l\ op\ r.
 *
 * Both operands may be placeholders, nested nodes, or plain values.
 *
 * @tparam L Left operand type.
 * @tparam R Right operand type.
 * @tparam Op Operator tag.
 */
template <class L, class R, class Op>
struct _binary {
    using _placeholder_expr = void;

    L l;
    R r;

    _binary(L l_, R r_)
        : l(static_cast<L &&>(l_)), r(static_cast<R &&>(r_)) {}

    template <class... Args>
    auto operator()(Args &&... args)
        -> decltype(static_cast<const _binary &>(*this)(
            std::forward<Args>(args)...)) {
        return static_cast<const _binary &>(*this)(
            std::forward<Args>(args)...);
    }

    template <class... Args>
    auto operator()(Args &&... args) const
        -> decltype(Op::apply(_eval<const L &, Args...>::get(l, args...),
                              _eval<const R &, Args...>::get(r, args...))) {
        return Op::apply(_eval<const L &, Args...>::get(l, args...),
                         _eval<const R &, Args...>::get(r, args...));
    }

    /**
     * @brief Applies a subscript to the result of this node.
     * @tparam T Index type.
     * @param t Index value.
     * @return A subscript node.
     */
    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<_binary, _d<T>, _op_subscript> operator[](T &&t) const {
        return {*this, std::forward<T>(t)};
    }
};

/**
 * @brief Short-circuiting logical-and node.
 *
 * @c _op_land is specialized so the right operand is evaluated only when the
 * left operand converts to @c true. This is the one place where the node
 * cannot use @c Op::apply, because that would evaluate both sides.
 *
 * @tparam L Left operand type.
 * @tparam R Right operand type.
 */
template <class L, class R>
struct _binary<L, R, _op_land> {
    using _placeholder_expr = void;

    L l;
    R r;

    _binary(L l_, R r_)
        : l(static_cast<L &&>(l_)), r(static_cast<R &&>(r_)) {}

    template <class... Args>
    bool operator()(Args &&... args) const {
        if (!static_cast<bool>(_eval<const L &, Args...>::get(l, args...))) {
            return false;
        }
        return static_cast<bool>(_eval<const R &, Args...>::get(r, args...));
    }

    template <class... Args>
    bool operator()(Args &&... args) {
        return static_cast<const _binary &>(*this)(
            std::forward<Args>(args)...);
    }

    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<_binary, _d<T>, _op_subscript> operator[](T &&t) const {
        return {*this, std::forward<T>(t)};
    }
};

/**
 * @brief Short-circuiting logical-or node.
 *
 * The right operand is evaluated only when the left operand converts to
 * @c false.
 *
 * @tparam L Left operand type.
 * @tparam R Right operand type.
 */
template <class L, class R>
struct _binary<L, R, _op_lor> {
    using _placeholder_expr = void;

    L l;
    R r;

    _binary(L l_, R r_)
        : l(static_cast<L &&>(l_)), r(static_cast<R &&>(r_)) {}

    template <class... Args>
    bool operator()(Args &&... args) const {
        if (static_cast<bool>(_eval<const L &, Args...>::get(l, args...))) {
            return true;
        }
        return static_cast<bool>(_eval<const R &, Args...>::get(r, args...));
    }

    template <class... Args>
    bool operator()(Args &&... args) {
        return static_cast<const _binary &>(*this)(
            std::forward<Args>(args)...);
    }

    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<_binary, _d<T>, _op_subscript> operator[](T &&t) const {
        return {*this, std::forward<T>(t)};
    }
};

/**
 * @brief Unary expression node.
 * @tparam X Operand type.
 * @tparam Op Operator tag.
 */
template <class X, class Op>
struct _unary {
    using _placeholder_expr = void;

    X x;

    _unary(X x_) : x(static_cast<X &&>(x_)) {}

    template <class... Args>
    auto operator()(Args &&... args)
        -> decltype(static_cast<const _unary &>(*this)(
            std::forward<Args>(args)...)) {
        return static_cast<const _unary &>(*this)(
            std::forward<Args>(args)...);
    }

    template <class... Args>
    auto operator()(Args &&... args) const
        -> decltype(Op::apply(_eval<const X &, Args...>::get(x, args...))) {
        return Op::apply(_eval<const X &, Args...>::get(x, args...));
    }

    template <class T,
              class = typename std::enable_if<_bindable<T>::value>::type>
    _binary<_unary, _d<T>, _op_subscript> operator[](T &&t) const {
        return {*this, std::forward<T>(t)};
    }
};

/**
 * @brief Defines a free binary operator for placeholder expressions.
 *
 * Enabled only when at least one operand is a placeholder or expression node,
 * so built-in and user-defined operators are unaffected. @c _bindable rejects
 * operands that cannot be stored by value, such as @c std::ostream.
 *
 * @param name Operator tag name.
 * @param op Operator token.
 */
#define PLACEHOLDER_BINOP(name, op)                                         \
    template <class L, class R,                                             \
              class = typename std::enable_if<                              \
                  (_is_arg<L>::value || _is_expr<L>::value ||               \
                   _is_arg<R>::value || _is_expr<R>::value) &&              \
                  _bindable<L>::value && _bindable<R>::value>::type>        \
    _binary<_d<L>, _d<R>, _op_##name> operator op(L &&l, R &&r) {           \
        return {std::forward<L>(l), std::forward<R>(r)};                    \
    }

/**
 * @brief Defines a free prefix unary operator for placeholder expressions.
 * @param name Operator tag name.
 * @param op Operator token.
 */
#define PLACEHOLDER_UNOP(name, op)                                          \
    template <class X,                                                      \
              class = typename std::enable_if<                              \
                  _is_operand<X>::value && _bindable<X>::value>::type>      \
    _unary<_d<X>, _op_##name> operator op(X &&x) {                          \
        return {std::forward<X>(x)};                                        \
    }

/**
 * @brief Defines a free postfix unary operator for placeholder expressions.
 * @param name Operator tag name.
 * @param op Operator token.
 */
#define PLACEHOLDER_POSTOP(name, op)                                        \
    template <class X,                                                      \
              class = typename std::enable_if<                              \
                  _is_operand<X>::value && _bindable<X>::value>::type>      \
    _unary<_d<X>, _op_##name> operator op(X &&x, int) {                     \
        return {std::forward<X>(x)};                                        \
    }

PLACEHOLDER_BINOP(add, +)
PLACEHOLDER_BINOP(sub, -)
PLACEHOLDER_BINOP(mul, *)
PLACEHOLDER_BINOP(div, /)
PLACEHOLDER_BINOP(mod, %)
PLACEHOLDER_BINOP(eq, ==)
PLACEHOLDER_BINOP(ne, !=)
PLACEHOLDER_BINOP(lt, <)
PLACEHOLDER_BINOP(le, <=)
PLACEHOLDER_BINOP(gt, >)
PLACEHOLDER_BINOP(ge, >=)
PLACEHOLDER_BINOP(land, &&)
PLACEHOLDER_BINOP(lor, ||)
PLACEHOLDER_BINOP(band, &)
PLACEHOLDER_BINOP(bor, |)
PLACEHOLDER_BINOP(bxor, ^)
PLACEHOLDER_BINOP(shl, <<)
PLACEHOLDER_BINOP(shr, >>)
PLACEHOLDER_BINOP(arrowstar, ->*)

PLACEHOLDER_UNOP(neg, -)
PLACEHOLDER_UNOP(pos, +)
PLACEHOLDER_UNOP(lnot, !)
PLACEHOLDER_UNOP(bnot, ~)
PLACEHOLDER_UNOP(deref, *)
PLACEHOLDER_UNOP(preinc, ++)
PLACEHOLDER_UNOP(predec, --)

PLACEHOLDER_POSTOP(postinc, ++)
PLACEHOLDER_POSTOP(postdec, --)

}  // namespace placeholder

#undef PLACEHOLDER_BINOP
#undef PLACEHOLDER_UNOP
#undef PLACEHOLDER_POSTOP
#undef PLACEHOLDER_ARG_MEMBER
#undef PLACEHOLDER_TAG2
#undef PLACEHOLDER_TAG1
#undef PLACEHOLDER_TAGPOST
#undef PLACEHOLDER_ARG_OBJECT
#undef PLACEHOLDER_ARG_TAG
#undef PLACEHOLDER_ARG_INDEX
#undef PLACEHOLDER_ARG_DECL

#endif  // PLACEHOLDER_HPP
