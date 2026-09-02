#include "advanced.h"

bool extractRoots(ModEntry primeEntry) {
    return false;
}

bool tryThisPrime(ModEntry primeEntry) {
    const uint64_t PRIME = primeEntry.modulus;
    if (PRIME <= 3) {
        // add all residues
        return true;
    }
    return extractRoots(primeEntry);
}

bool trySmallPowersOfThisPrime(uint64_t prime,
    ModEntryWrapper** restrict firstModEntryWrapper,
    ModEntryWrapper** restrict lastModEntryWrapper) {
    ModEntry firstModEntry = primeModEntry(prime);

    if (!tryThisPrime(firstModEntry)) {return false;}

    ModEntry currentModEntry = firstModEntry;
    ModEntryWrapper currentModEntryWrapper = makeModEntryWrapper(currentModEntry, NULL);

    // while (currentMod <= SQRT_DIVBOUND) {

    // }
    *firstModEntryWrapper = malloc(sizeof(ModEntryWrapper));
    **firstModEntryWrapper = makeModEntryWrapper(firstModEntry, NULL);
    *lastModEntryWrapper = malloc(sizeof(ModEntryWrapper));
    **lastModEntryWrapper = currentModEntryWrapper;
    return true;
}

bool trySmallPowersOfSmallPrimes(primesieve_iterator* primeIterator,
    PrimeWrapper** primeWrapperHead) {
    ModEntryWrapper* firstModEntryWrapper = NULL,* lastModEntryWrapper = NULL;
    PrimeWrapper* primeWrapper = NULL;
    uint64_t prime;
    while ((prime = primesieve_next_prime(primeIterator)) <= SQRT_DIVBOUND) {      
        if (trySmallPowersOfThisPrime(prime, &firstModEntryWrapper, &lastModEntryWrapper)) {
            primeWrapper = malloc(sizeof(PrimeWrapper));
            *primeWrapper = makePrimeWrapper(firstModEntryWrapper,
                lastModEntryWrapper,
                *primeWrapperHead);
            *primeWrapperHead = primeWrapper;
        }
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