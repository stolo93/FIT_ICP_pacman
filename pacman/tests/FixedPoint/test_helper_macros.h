#ifndef PACMAN_TEST_HELPER_MACROS_H
#define PACMAN_TEST_HELPER_MACROS_H

#include <iostream>

#ifndef OVERFLOW_CHECKS
#    define OVERFLOW_CHECKS
#endif

#define ASSERT_THROWS_DOMAIN_ERROR(code)                                                                               \
    {                                                                                                                  \
        try {                                                                                                          \
            (code);                                                                                                    \
            std::cerr << "Failed to throw domain_error: " << #code << std::endl;                                       \
            return 1;                                                                                                  \
        } catch (std::domain_error & e) {                                                                              \
        }                                                                                                              \
    }

#define ASSERT(condition)                                                                                              \
    if (! (condition)) {                                                                                               \
        std::cerr << "Assertion failed: " << #condition << std::endl;                                                  \
        return 1;                                                                                                      \
    }
#endif    // PACMAN_TEST_HELPER_MACROS_H
