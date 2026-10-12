#include "wrappers.h"

ResidueWrapper makeResidueWrapper(uint64_t residue, ResidueWrapper* prev) {
  return (ResidueWrapper){residue, prev};
}

ModulusWrapper makeModulusWrapper(libmontmul_t montmul_t, ModulusWrapper* prev) {
  return (ModulusWrapper){montmul_t, NULL, prev};
}

PrimeWrapper makePrimeWrapper(ModulusWrapper* restrict first, ModulusWrapper* restrict last, PrimeWrapper* prev) {
  return (PrimeWrapper){first, last, prev};
}

void appendResidue(ModulusWrapper* modEntryWrapper, uint64_t residue) {
  ResidueWrapper* prev = modEntryWrapper->residueWrapper;
  modEntryWrapper->residueWrapper = malloc(sizeof(ResidueWrapper));
  *(modEntryWrapper->residueWrapper) = makeResidueWrapper(residue, prev);
}

void freeResidueWrappers(ResidueWrapper* head) {
  ResidueWrapper* temp;
  while (head != NULL) {
    temp = head->prev;
    (void)free(head);
    head = temp;
  }
}

void freeModulusWrappers(ModulusWrapper* head) {
  ModulusWrapper* temp;
  while (head != NULL) {
    temp = head->prev;
    (void)freeResidueWrappers(head->residueWrapper);
    (void)free(head);
    //putchar('\n');
    head = temp;
  }
}

void freePrimeWrappers(PrimeWrapper* head) {
  PrimeWrapper* temp;
  while (head != NULL) {
    temp = head->prev;
    (void)free(head->first);
    (void)freeModulusWrappers(head->last);
    (void)free(head);
    head = temp;
  }
}
