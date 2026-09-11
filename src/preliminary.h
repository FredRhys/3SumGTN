#ifndef PRELIMINARY_H
#define PRELIMINARY_H

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "formula.h"

#define UINT21_MAX 0x1FFFFF

extern FILE* resultsDotTxt;

bool tryPreliminary(uint64_t k);

#endif