#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdlib.h>
#include <inttypes.h>
#include "montmul.h"

typedef struct libmontmul_t libmontmul_t;

typedef struct ResidueWrapper {
  uint64_t residue;
  struct ResidueWrapper* prev; // added in incidence order
}ResidueWrapper;

typedef struct ModulusWrapper {
  libmontmul_t modulus;
  ResidueWrapper* residueWrapper;
  struct ModulusWrapper* prev; // the ModEntryWrapper head should be that with the greatest modulus.
}ModulusWrapper;

typedef struct PrimeWrapper {
  ModulusWrapper* first;
  ModulusWrapper* last;
  struct PrimeWrapper* prev; // the PrimeWrapper head should be that with the greatest modulus.
}PrimeWrapper;

ResidueWrapper makeResidueWrapper(uint64_t residue, ResidueWrapper* prev);
ModulusWrapper makeModulusWrapper(libmontmul_t libmontmul_t, ModulusWrapper* prev);
PrimeWrapper makePrimeWrapper(ModulusWrapper* restrict first, ModulusWrapper* restrict last, PrimeWrapper* prev);
void appendResidue(ModulusWrapper* modEntryWrapper, uint64_t residue);
void freeResidueWrappers(ResidueWrapper* head);
void freeModulusWrappers(ModulusWrapper* head);
void freePrimeWrappers(PrimeWrapper* head);

#endif
