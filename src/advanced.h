#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul.h" // this is included by poly.h
#include <stdbool.h>
#include <stdint.h>

// global variables
extern FILE* resultsDotTxt;

bool tryAdvanced(uint64_t k);

#endif