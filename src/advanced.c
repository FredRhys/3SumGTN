#include "advanced.h"

bool extractRootsFromQuad(Poly poly, ModEntryWrapper* modEntryWrapper) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    const uint64_t A = poly.deg2, B = poly.deg1, C = poly.deg0;
    const uint64_t DENOMINATOR = invmod(montmul(2, A, MOD_ENTRY), MOD_ENTRY);
    const uint64_t DISCRIMINANT = submod(montexp(B, 2, MOD_ENTRY), montmul(4, montmul(A, C, MOD_ENTRY), MOD_ENTRY), PRIME);
    if (legendre(DISCRIMINANT, MOD_ENTRY) == -1) {return false;}
    const uint64_t SQRT = sqrtmod(DISCRIMINANT, MOD_ENTRY);
    const uint64_t NUMERATOR1 = submod(SQRT, B, PRIME);
    const uint64_t NUMERATOR2 = submod(PRIME - SQRT, B, PRIME);
    const uint64_t ROOT1 = montmul(NUMERATOR1, DENOMINATOR, MOD_ENTRY);
    appendResidue(modEntryWrapper, ROOT1);
    if (NUMERATOR1 == NUMERATOR2) {return true;}
    const uint64_t ROOT2 = montmul(NUMERATOR2, DENOMINATOR, MOD_ENTRY);
    appendResidue(modEntryWrapper, ROOT2);
    return true;
}

bool extractRoots(Poly poly, ModEntryWrapper* modEntryWrapper) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    switch (degreeOfPoly(poly)) {
        case 2:
            return extractRootsFromQuad(poly, modEntryWrapper);
        case 1:
            uint64_t root = submod(0, poly.deg0, PRIME);
            appendResidue(modEntryWrapper, root);
            return true;
        default:
            return false;
    }

}

bool extractRootsRunner(ModEntryWrapper* modEntryWrapper, uint64_t _6k) {
    Poly rootPoly;
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    for (uint64_t offset = 0; offset < PRIME; offset++) {
        rootPoly = getRootPoly(MOD_ENTRY, _6k, offset);
        if (degreeOfPoly(rootPoly) == 0) {continue;}
        const Poly GCD1 = gcdPoly(rootPoly, MOD_ENTRY, _6k);
        rootPoly.deg0 = submod(rootPoly.deg0, 2, PRIME);
        const Poly GCD2 = gcdPoly(rootPoly, MOD_ENTRY, _6k);
        fprintf(stdout, "%"PRIu64": %d\n", PRIME, degreeOfPoly(GCD1) + degreeOfPoly(GCD2));
        switch (degreeOfPoly(GCD1) + degreeOfPoly(GCD2)) {
            case 2:
                appendResidue(modEntryWrapper, offset);
                break;
            case 0:
                if (applyMasterPoly(offset, MOD_ENTRY, _6k) != 0) {break;}
                appendResidue(modEntryWrapper, offset);
                break;
            default:
                break;
        }
        const bool RESULT1 = extractRoots(GCD1, modEntryWrapper);
        return extractRoots(GCD2, modEntryWrapper) || RESULT1;
    }
    return false;
}

bool tryThisPrime(ModEntryWrapper* modEntryWrapper, uint64_t _6k) {
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
            return extractRootsRunner(modEntryWrapper, _6k);
    }    
}

bool trySmallPowersOfThisPrime(uint64_t prime, uint64_t _6k, ModEntryWrapper** restrict firstPtr, ModEntryWrapper** restrict lastPtr) {
    ModEntry primeEntry = primeModEntry(prime);
    ModEntryWrapper first = makeModEntryWrapper(primeEntry, NULL);
    if (!tryThisPrime(&first, _6k)) {return false;}

    // exponent loop

    *firstPtr = malloc(sizeof(ModEntryWrapper));
    **firstPtr = first;
    *lastPtr = malloc(sizeof(ModEntryWrapper));
    **lastPtr = first;
    return true;
}

bool trySmallPowersOfSmallPrimes(uint64_t _6k, primesieve_iterator* primeIterator, PrimeWrapper** primeWrapperHead) {
    PrimeWrapper* temp;
    ModEntryWrapper* firstModEntryWrapper;
    ModEntryWrapper* lastModEntryWrapper;
    uint64_t prime;
    while ((prime = primesieve_next_prime(primeIterator)) <= SQRT_DIVBOUND) {
        if (!trySmallPowersOfThisPrime(prime, _6k, &firstModEntryWrapper, &lastModEntryWrapper)) {continue;}        
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
    const uint64_t _6k = 6 * k;
    
    if (trySmallPowersOfSmallPrimes(_6k, &primeIterator, &primeWrapperHead)) {
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