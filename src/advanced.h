#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul/montmul.h" // this is included by poly.h
#include "../libdivide/libdivide.h"
#include "wrappers.h"
#include "formula.h"
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include <inttypes.h>

#define UINT42_MAX 0x3FFFFFFFFFF

<<<<<<< HEAD
bool tryAdvanced(uint64_t k, int64_t sqrtDivbound);
=======
typedef struct libdivide_s64_t libdivide_s64_t;
bool tryAdvanced(uint64_t k);
>>>>>>> dev

#endif