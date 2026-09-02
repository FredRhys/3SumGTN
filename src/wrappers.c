#include "wrappers.h"
ResidueWrapper makeResidueWrapper(uint64_t residue) {
    return (ResidueWrapper){residue, NULL};
}

ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev) {
    return (ModEntryWrapper){modEntry, NULL, prev};
}

PrimeWrapper makePrimeWrapper(ModEntryWrapper* restrict first,
    ModEntryWrapper* restrict last,
    PrimeWrapper* prev) {
    return (PrimeWrapper){first, last, prev};
}