#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdlib.h>
#include "../libmontmul/montmul.h"

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
    ModEntryWrapper* firstModEntryWrapper;
    ModEntryWrapper* lastModEntryWrapper;
    struct PrimeWrapper* prev; // the PrimeWrapper head should be that with the greatest modulus.
}PrimeWrapper;

ResidueWrapper makeResidueWrapper(uint64_t residue);
ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev);
PrimeWrapper makePrimeWrapper(ModEntryWrapper* restrict first, ModEntryWrapper* restrict last, PrimeWrapper* prev);
void freePrimeWrappers(PrimeWrapper* head);

#endif