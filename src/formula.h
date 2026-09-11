#ifndef FORMULA_H
#define FORMULA_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdbit.h>

#define UINT21_MAX 0x1FFFFF

// global variables
extern FILE* resultsDotTxt;

__uint128_t abs128(__int128_t operand);
void fprintfResults(uint64_t x, uint64_t y, uint64_t z, uint64_t k);
bool checkFormulaResults(__int128_t dividend, uint64_t divisor, uint64_t _3d, uint64_t k, int64_t z);

#endif