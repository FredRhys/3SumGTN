#ifndef PRELIMINARY_H
#define PRELIMINARY_H

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "formula.h"

extern FILE* resultsDotTxt;

bool tryPreliminary(uint64_t k, int64_t bound, int64_t oldBound);

#endif
