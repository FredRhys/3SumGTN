#ifndef FORMULA_H
#define FORMULA_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdbit.h>
#include "../libdivide/libdivide.h"

// global variables
extern FILE* resultsDotTxt;

typedef struct libdivide_s64_t libdivide_s64_t;

__uint128_t abs128(__int128_t operand);
void fprintfResults(uint64_t x, uint64_t y, uint64_t z, uint64_t k);
bool checkFormulaResults(__int128_t dividend, uint64_t divisor, libdivide_s64_t* tripleDivisor, uint64_t k, int64_t z);

#endif