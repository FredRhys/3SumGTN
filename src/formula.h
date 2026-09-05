#ifndef FORMULA_H
#define FORMULA_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdbit.h>

// global variables
extern FILE* resultsDotTxt;

__uint128_t abs128(__int128_t operand);
bool checkFormulaResults(__int128_t dividend, uint64_t divisor, uint64_t _3d, uint64_t k, int64_t z);

#endif