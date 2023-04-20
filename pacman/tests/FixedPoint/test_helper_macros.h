#ifndef PACMAN_TEST_HELPER_MACROS_H
#define PACMAN_TEST_HELPER_MACROS_H
#define ASSERT_THROWS_DOMAIN_ERROR(code)                                                                               \
    {                                                                                                                  \
        try {                                                                                                          \
            (code);                                                                                                    \
            return 1;                                                                                                  \
        } catch (std::domain_error & e) {                                                                              \
        }                                                                                                              \
    }

#define ASSERT(condition)                                                                                              \
    if (! (condition)) { return 1; }
#endif    // PACMAN_TEST_HELPER_MACROS_H
