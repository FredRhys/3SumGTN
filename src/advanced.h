#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include <inttypes.h>
#include "../libmontmul/montmul.h" // this is included by poly.h
#include "../libdivide/libdivide.h"
#include "poly.h"
#include "wrappers.h"
#include "formula.h"

#define UINT42_MAX 0x3FFFFFFFFFF

typedef struct libdivide_u64_t libdivide_u64_t;
bool tryAdvanced(uint64_t k, int64_t sqrtDivbound, int64_t oldDivbound);

#endif
