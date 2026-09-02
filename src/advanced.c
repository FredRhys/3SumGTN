#include "advanced.h"

ResidueWrapper makeResidueWrapper(uint64_t residue) {
    return (ResidueWrapper){residue, NULL};
}

ModEntryWrapper makeModEntryWrapper(ModEntry modEntry, ModEntryWrapper* prev) {
    return (ModEntryWrapper){modEntry, NULL, prev};
}

PrimeWrapper makePrimeWrapper(uint64_t prime,
    ModEntryWrapper* restrict first,
    ModEntryWrapper* restrict last,
    PrimeWrapper* prev) {
    return (PrimeWrapper){prime, first, last, prev};
}

void freePrimeWrappers(PrimeWrapper* head) {
    PrimeWrapper* temp;
    while (head != NULL) {
        temp = head->prev;
        free(head);
        head = temp;
    }
}

bool trySmallPowersOfThisPrime() {
    return true;
}

bool trySmallPowersOfSmallPrimes(primesieve_iterator primeIterator,
    PrimeWrapper** primeWrapperHead) {
    uint64_t prime;
    while ((prime = primesieve_next_prime(&primeIterator)) <= SQRT_DIVBOUND) {
        ModEntryWrapper* firstModEntryWrapper = NULL,* lastModEntryWrapper = NULL;
        if (trySmallPowersOfThisPrime()) {
            PrimeWrapper* primeWrapper = malloc(sizeof(PrimeWrapper));
            *primeWrapper = makePrimeWrapper(prime,
                firstModEntryWrapper,
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
    
    if (trySmallPowersOfSmallPrimes(primeIterator, &primeWrapperHead)) {
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