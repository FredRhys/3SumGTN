#ifndef ADVANCED_H
#define ADVANCED_H
#include <primesieve.h>
#include "poly.h"
#include "../libmontmul/montmul.h" // this is included by poly.h
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define UINT21_MAX 0x1FFFFF
#define DIVBOUND 545094
#define SQRT_DIVBOUND 739

typedef struct ResidueWrapper {
    uint64_t residue;
    struct ResidueWrapper* next; // added in incidence order
}ResidueWrapper;

typedef struct ModEntryWrapper {
    ModEntry modEntry;
    ResidueWrapper* residueHead;
    struct ModEntryWrapper* prev; // the ModEntryWrapper head should be that with the greatest modulus.
}ModEntryWrapper;

typedef struct PrimeWrapper {
    uint64_t prime;
    ModEntryWrapper* firstModEntry;
    ModEntryWrapper* lastModEntry;
    struct PrimeWrapper* prev; // the PrimeWrapper head should be that with the greatest modulus.
}PrimeWrapper;

// global variables
extern FILE* resultsDotTxt;

bool tryAdvanced(uint64_t k);

#endif