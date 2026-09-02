#include "wrappers.h"
ResidueWrapper makeResidueWrapper(uint64_t residue, ResidueWrapper* next) {
    return (ResidueWrapper){residue, next};
}

ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev) {
    return (ModEntryWrapper){modEntry, NULL, prev};
}

PrimeWrapper makePrimeWrapper(ModEntryWrapper* restrict first,
    ModEntryWrapper* restrict last,
    PrimeWrapper* prev) {
    return (PrimeWrapper){first, last, prev};
}

void freePrimeWrappers(PrimeWrapper* head) {
    PrimeWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        (void)free(head->firstModEntryWrapper);
        (void)free(head->lastModEntryWrapper);
        (void)free(head);
        head = temp;
    }
}