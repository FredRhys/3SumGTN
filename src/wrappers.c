#include "wrappers.h"
#include <stdio.h>

ResidueWrapper makeResidueWrapper(uint64_t residue, ResidueWrapper* prev) {
    return (ResidueWrapper){residue, prev};
}

ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev) {
    return (ModEntryWrapper){modEntry, NULL, prev};
}

PrimeWrapper makePrimeWrapper(ModEntryWrapper* restrict first,
    ModEntryWrapper* restrict last,
    PrimeWrapper* prev) {
    return (PrimeWrapper){first, last, prev};
}

void appendResidue(ModEntryWrapper* modEntryWrapper, uint64_t residue) {
    ResidueWrapper* prev = modEntryWrapper->residueHead;
    modEntryWrapper->residueHead = malloc(sizeof(ResidueWrapper));
    *(modEntryWrapper->residueHead) = makeResidueWrapper(residue, prev);
}

void freeResidueWrappers(ResidueWrapper* head) {
    ResidueWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        //fprintf(stdout, "%"PRIu64", ", head->residue);
        (void)free(head);
        head = temp;
    }
}

void freeModEntryWrappers(ModEntryWrapper* head) {
    ModEntryWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        //fprintf(stdout, "%"PRIu64": ", head->modEntry.modulus);
        (void)freeResidueWrappers(head->residueHead);
        (void)free(head);
        //putchar('\n');
        head = temp;
    }
}

void freePrimeWrappers(PrimeWrapper* head) {
    PrimeWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        (void)free(head->firstModEntryWrapper);
        (void)freeModEntryWrappers(head->lastModEntryWrapper);
        (void)free(head);
        head = temp;
    }
}