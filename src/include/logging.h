#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define printd(...) printf(__VA_ARGS__)
#else
void()
#endif

#endif // LOGGING_H
