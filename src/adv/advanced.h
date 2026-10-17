#ifndef ADVANCED_H
#define ADVANCED_H

#include <primesieve.h>
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include <inttypes.h>
#include "montmul.h" // this is included by poly.h
#include "libdivide.h"
#include "poly.h"
#include "wrappers.h"
#include "formula.h"
#include "returnif.h"

#define UINT42_MAX 0x3FFFFFFFFFF

typedef struct bounds_t {
    int64_t bound;
    int64_t divbound;
    int64_t sqrtDivbound;
    int64_t oldBound;
    int64_t oldDivbound;
} bounds_t;

typedef struct libmontmul_t libmontmul_t;

typedef struct libdivide_u64_t libdivide_u64_t;
bool tryAdvanced(uint64_t k, bounds_t bounds);

#endif
