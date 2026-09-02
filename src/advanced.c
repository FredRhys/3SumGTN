#include "advanced.h"

bool tryThisPrime(uint64_t prime) {
    if (prime <= 3) {
        // add all residues
        return true;
    }
    return extractRoots(prime);
}

bool trySmallPowersOfThisPrime(uint64_t prime,
    ModEntryWrapper** restrict firstModEntryWrapper,
    ModEntryWrapper** restrict lastModEntryWrapper) {
    ModEntry firstModEntry = primeModEntry(prime);
    //ModEntry currentModEntry = firstModEntry;

    if (!tryThisPrime(prime)) {return false;}

    // while (currentMod <= SQRT_DIVBOUND) {

    // }
    *firstModEntryWrapper = malloc(sizeof(ModEntryWrapper));
    *lastModEntryWrapper = malloc(sizeof(ModEntryWrapper));
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