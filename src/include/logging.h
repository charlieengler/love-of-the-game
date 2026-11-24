#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define printd(...)                                                                                                                                                                                    \
    printf("%s->%s[%d]: ", __FILE__, __func__, __LINE__);                                                                                                                                              \
    printf(__VA_ARGS__)
#else
#define printd(...) asm("nop")
#endif

#endif // LOGGING_H
