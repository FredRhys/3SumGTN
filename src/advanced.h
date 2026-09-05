#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul/montmul.h" // this is included by poly.h
#include "wrappers.h"
#include "formula.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define UINT21_MAX 0x1FFFFF
#define UINT42_MAX 0x3FFFFFFFFFF
#define DIVBOUND 545094
#define SQRT_DIVBOUND 739

bool tryAdvanced(uint64_t k);

#endif