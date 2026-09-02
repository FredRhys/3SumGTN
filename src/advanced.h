#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul/montmul.h" // this is included by poly.h
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define UINT21_MAX 0x1FFFFF
#define DIVBOUND 545094
#define SQRT_DIVBOUND 739

// global variables
extern FILE* resultsDotTxt;

bool tryAdvanced(uint64_t k);

#endif