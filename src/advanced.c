#include "advanced.h"

ResidueWrapper makeResidueWrapper(uint64_t residue) {
    return (ResidueWrapper){residue, NULL};
}

ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev) {
    return (ModEntryWrapper){modEntry, NULL, prev};
}

PrimeWrapper makePrimeWrapper(uint64_t prime, PrimeWrapper* prev) {
    return (PrimeWrapper){prime, NULL, NULL, prev};
}

bool tryAdvanced(uint64_t k) {
    primesieve_iterator primeIterator;
    primesieve_init(&primeIterator);
    
    primesieve_free_iterator(&primeIterator);
    return false;
}