#pragma once

#define GLUE(x, y) x y

#define ARGS_COUNT(a1, a2, a3, a4, a5, count, ...) count
#define EXPAND_ARGS(args) ARGS_COUNT args
#define COUNT_ARGS_MAX5(...) EXPAND_ARGS((__VA_ARGS__, 5, 4, 3, 2, 1, 0))

#define OVERLOAD_MACRO(name, count) OVERLOAD_MACRO1(name, count)
#define OVERLOAD_MACRO1(name, count) OVERLOAD_MACRO2(name, count)
#define OVERLOAD_MACRO2(name, count) name##count

#define CALL_OVERLOAD(name, ...)                                                                   \
    GLUE(OVERLOAD_MACRO(name, COUNT_ARGS_MAX5(__VA_ARGS__)), (__VA_ARGS__))

#ifdef _DEBUG
#    define DEBUG_ONLY(expr) expr
#else
#    define DEBUG_ONLY(expr)
#endif
