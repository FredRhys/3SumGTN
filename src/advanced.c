#include "advanced.h"

bool extractRoots(ModEntry primeEntry) {
    return false;
}

void appendResidue(ModEntryWrapper* modEntryWrapper, uint64_t residue) {
    ResidueWrapper* prev = modEntryWrapper->residueHead;
    modEntryWrapper->residueHead = malloc(sizeof(ResidueWrapper));
    *(modEntryWrapper->residueHead) = makeResidueWrapper(residue, prev);
}

bool tryThisPrime(ModEntryWrapper* modEntryWrapper) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    switch (PRIME) {
        case 3:
            appendResidue(modEntryWrapper, 2);
        case 2:
            appendResidue(modEntryWrapper, 1);
            appendResidue(modEntryWrapper, 0);
            return true;
        default:
            return extractRoots(MOD_ENTRY);
    }    
}

bool trySmallPowersOfThisPrime(uint64_t prime, ModEntryWrapper** restrict firstPtr, ModEntryWrapper** restrict lastPtr) {
    ModEntry primeEntry = primeModEntry(prime);
    ModEntryWrapper first = makeModEntryWrapper(primeEntry, NULL);
    if (!tryThisPrime(&first)) {return false;}

    // exponent loop

    *firstPtr = malloc(sizeof(ModEntryWrapper));
    **firstPtr = first;
    *lastPtr = malloc(sizeof(ModEntryWrapper));
    **lastPtr = first;
    return true;
}

bool trySmallPowersOfSmallPrimes(primesieve_iterator* primeIterator,
    PrimeWrapper** primeWrapperHead) {
    PrimeWrapper* temp;
    ModEntryWrapper* firstModEntryWrapper;
    ModEntryWrapper* lastModEntryWrapper;
    uint64_t prime;
    while ((prime = primesieve_next_prime(primeIterator)) <= SQRT_DIVBOUND) {
        if (!trySmallPowersOfThisPrime(prime, &firstModEntryWrapper, &lastModEntryWrapper)) {continue;}        
        temp = *primeWrapperHead;
        *primeWrapperHead = malloc(sizeof(PrimeWrapper));
        **primeWrapperHead = makePrimeWrapper(firstModEntryWrapper, lastModEntryWrapper, temp);
    }
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
    (void)primesieve_init(&primeIterator);
    PrimeWrapper* primeWrapperHead = NULL;
    
    if (trySmallPowersOfSmallPrimes(&primeIterator, &primeWrapperHead)) {
        return true;
    }
    if (tryLargePowersOfSmallPrimes()) {
        return true;
    }
    if (tryLargePrimes()) {
        return true;
    }

    (void)freePrimeWrappers(primeWrapperHead);
    (void)primesieve_free_iterator(&primeIterator);
    return false;
}