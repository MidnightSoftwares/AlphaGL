#pragma once

#include "Helpers.h"

#include <cstdlib>

#define ASSERT(expr)                                                                               \
    if (expr) {                                                                                    \
    } else {                                                                                       \
        exit(EXIT_FAILURE);                                                                        \
    }

#define DEBUG_ASSERT(expr) DEBUG_ONLY(ASSERT(expr))
