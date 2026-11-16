#ifndef FORMULA_H
#define FORMULA_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "libdivide.h"
#include "isQr.h"

#define UINT21_MAX 0x1FFFFF

// global variables
extern FILE* resultsDotTxt;

typedef struct libdivide_u64_t libdivide_u64_t;

__uint128_t abs128(__int128_t operand);

void fprintfResults(int64_t x, int64_t y, int64_t z, uint64_t k);
bool checkFormulaResults(__int128_t dividend, uint64_t divisor, libdivide_u64_t* tripleDivisor, uint64_t k, int64_t z);

#endif
