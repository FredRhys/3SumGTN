#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul/montmul.h" // this is included by poly.h
#include "wrappers.h"
#include "formula.h"
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include <inttypes.h>

#define UINT42_MAX 0x3FFFFFFFFFF

bool tryAdvanced(uint64_t k, uint64_t sqrtDivbound);

#endif