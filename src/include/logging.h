#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define printd(...) printf(__VA_ARGS__)
#else
#define printd(...) asm("nop")
#endif

#endif // LOGGING_H
