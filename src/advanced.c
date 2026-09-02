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

bool trySmallPowersOfSmallPrimes() {
    return false;
}

bool tryLargePowersOfSmallPrimes() {
    return false;
}

bool tryLargePrimes() {
    return false;
}

bool tryAdvanced(uint64_t k) {
    primesieve_iterator primeIterator;
    primesieve_init(&primeIterator);
    
    if (trySmallPowersOfSmallPrimes()) {
        return true;
    }
    if (tryLargePowersOfSmallPrimes()) {
        return true;
    }
    if (tryLargePrimes()) {
        return true;
    }

    primesieve_free_iterator(&primeIterator);
    return false;
}