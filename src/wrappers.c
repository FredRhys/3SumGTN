#include "wrappers.h"
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

void freeResidueWrappers(ResidueWrapper* head) {
    ResidueWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        (void)free(head);
        head = temp;
    }
}

void freeModEntryWrappers(ModEntryWrapper* head) {
    ModEntryWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        (void)freeResidueWrappers(head->residueHead);
        (void)free(head);
        head = temp;
    }
}

void freePrimeWrappers(PrimeWrapper* head) {
    PrimeWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        (void)freeModEntryWrappers(head->lastModEntryWrapper);
        (void)free(head);
        head = temp;
    }
}