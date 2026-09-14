#ifndef BASIC_H

#define BASIC_H
#define DIVISOR_LIM 1000

#include <stdbool.h>
#include <inttypes.h>
#include "formula.h"
#include "../libdivide/libdivide.h"

typedef struct libdivide_s64_t libdivide_s64_t;

// functions from factor64
int initfactor64(const char*);
int factor64(uint64_t*, int*, uint64_t);

bool tryBasic(uint64_t k);
bool tryBasicMax(uint64_t k);

#endif