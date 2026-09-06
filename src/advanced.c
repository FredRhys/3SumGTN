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
    const uint64_t NUMERATOR2 = submod(0, B + SQRT, PRIME);
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

void checkOffsetIsRoot(ModEntryWrapper* modEntryWrapper, uint64_t _6k, uint64_t offset, uint8_t degreeSum) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    switch (degreeSum) {
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
}

bool extractRootsFromPrime(ModEntryWrapper* modEntryWrapper, uint64_t k) {
    Poly rootPoly;
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    const uint64_t _6k = montmul(6, k, MOD_ENTRY);
    for (uint64_t offset = 0; offset < PRIME; offset++) {
        rootPoly = getRootPoly(MOD_ENTRY, _6k, offset);
        if (degreeOfPoly(rootPoly) == 0) {continue;}
        const Poly GCD1 = gcdPoly(rootPoly, MOD_ENTRY, _6k);
        rootPoly.deg0 = submod(rootPoly.deg0, 2, PRIME);
        const Poly GCD2 = gcdPoly(rootPoly, MOD_ENTRY, _6k);
        (void)checkOffsetIsRoot(modEntryWrapper, _6k, offset, degreeOfPoly(GCD1) + degreeOfPoly(GCD2));
        const bool RESULT1 = extractRoots(GCD1, modEntryWrapper);
        return extractRoots(GCD2, modEntryWrapper) || RESULT1;
    }
    return false;
}

bool tryThisPrime(ModEntryWrapper* modEntryWrapper, uint64_t k) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t PRIME = MOD_ENTRY.modulus;
    switch (PRIME) {
        case 3:
            appendResidue(modEntryWrapper, 2);
            // Employing intentional case fallthrough
        case 2:
            appendResidue(modEntryWrapper, 1);
            appendResidue(modEntryWrapper, 0);
            return true;
        default:
            return extractRootsFromPrime(modEntryWrapper, k);
    }    
}

void extractRootsWithHensels(ModEntryWrapper* modEntryWrapper, ModEntry primeEntry, uint64_t oldRoot, uint64_t denominator, uint64_t _6k) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t MODULUS = MOD_ENTRY.modulus;
    const uint64_t NEW_ROOT = submod(oldRoot,
        montmul(invmod(denominator, primeEntry), applyMasterPoly(oldRoot, MOD_ENTRY, _6k), MOD_ENTRY),
        MODULUS);
    appendResidue(modEntryWrapper, NEW_ROOT);
}

void extractRootsWithBruteForce(ModEntryWrapper* modEntryWrapper, uint64_t _6k) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t MODULUS = MOD_ENTRY.modulus;
    // TODO can determine number of roots with implemented tools
    for (uint64_t i = 0; i < MODULUS; i++) {
        if (applyMasterPoly(i, MOD_ENTRY, _6k) != 0) {continue;}
        appendResidue(modEntryWrapper, i);
    }
}

void extractRootsFromPower(ModEntryWrapper* modEntryWrapper, ResidueWrapper* residueHead, ModEntry primeEntry, uint64_t k) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t _6k = montmul(6, k, MOD_ENTRY);
    uint64_t residue;
    while (residueHead != NULL) {
        residue = residueHead->residue;
        const uint64_t DENOMINATOR = applyMasterPolyDeriv(residue, MOD_ENTRY);
        //TODO figure out why this doesn't accept modulus of 4.
        if (DENOMINATOR != 0 && MOD_ENTRY.modulus != 4) {
            (void)extractRootsWithHensels(modEntryWrapper, primeEntry, residue, DENOMINATOR, _6k);
        }
        else {
            (void)extractRootsWithBruteForce(modEntryWrapper, _6k);
            break;
        }
        residueHead = residueHead->prev;
    }
}

bool tryPowersOfThisPrime(uint64_t prime, uint64_t k, ModEntryWrapper** restrict firstPtr, ModEntryWrapper** restrict lastPtr) {
    ModEntry primeEntry = primeModEntry(prime);
    ModEntryWrapper first = makeModEntryWrapper(primeEntry, NULL);
    if (!tryThisPrime(&first, k)) {return false;}
    ResidueWrapper* residueHead = first.residueHead;
    *firstPtr = malloc(sizeof(ModEntryWrapper));
    **firstPtr = first;
    *lastPtr = malloc(sizeof(ModEntryWrapper));
    **lastPtr = first;
    ModEntryWrapper last;
    uint64_t modulus = prime * prime;
    ModEntry modEntry = increasePrimeModEntryPower(primeEntry, primeEntry);
    while (modulus < DIVBOUND) {
        last = makeModEntryWrapper(modEntry, *lastPtr);
        (void)extractRootsFromPower(&last, residueHead, primeEntry, k);
        *lastPtr = malloc(sizeof(ModEntryWrapper));
        **lastPtr = last;
        modulus *= prime;
        modEntry = increasePrimeModEntryPower(modEntry, primeEntry);
        residueHead = last.residueHead;
    }
    return true;
}

bool checkResidue(uint64_t modulus, uint64_t k, uint64_t residue, int8_t SIGN) {
    const int64_t INCREMENT = SIGN * modulus;
    for (int64_t z = residue; (SIGN) * z < DIVBOUND; z += INCREMENT) {
        if ((SIGN) * z > UINT42_MAX) {break;}
        __int128_t dividand = (__int128_t)z * z * z - z - 6 * k;
        if (checkFormulaResults(dividand, modulus, 3 * modulus, k, z)) {
            return true;
        }
    }
    return false;
}

bool checkResidueRunner(uint64_t residue, uint64_t modulus, uint64_t k) {
    return checkResidue(modulus, k, residue, 1) || checkResidue(modulus, k, residue, -1);
}

bool checkAllResidues(PrimeWrapper* primeWrapper, uint64_t k) {
    ModEntryWrapper* modEntryWrapper;
    ModEntry modEntry;
    ResidueWrapper* residueWrapper;
    uint64_t modulus;
    uint64_t residue;
    while (primeWrapper != NULL) {
        modEntryWrapper = primeWrapper->lastModEntryWrapper;
        while (modEntryWrapper != NULL) {
            modEntry = modEntryWrapper->modEntry;
            modulus = modEntry.modulus;
            residueWrapper = modEntryWrapper->residueHead;
            while (residueWrapper != NULL) {
                residue = residueWrapper->residue;
                if (checkResidueRunner(residue, modulus, k)) {
                    return true;
                }
                residueWrapper = residueWrapper->prev;
            }
            modEntryWrapper = modEntryWrapper->prev;
        }
        
        primeWrapper = primeWrapper->prev;
    }
    return false;
}

bool tryPowersOfSmallPrimes(uint64_t k, primesieve_iterator* primeIterator, PrimeWrapper** primeWrapper) {
    PrimeWrapper* temp;
    ModEntryWrapper* firstModEntryWrapper;
    ModEntryWrapper* lastModEntryWrapper;
    uint64_t prime;
    while ((prime = primesieve_next_prime(primeIterator)) < SQRT_DIVBOUND) {
        if (!tryPowersOfThisPrime(prime, k, &firstModEntryWrapper, &lastModEntryWrapper)) {continue;}        
        temp = *primeWrapper;
        *primeWrapper = malloc(sizeof(PrimeWrapper));
        **primeWrapper = makePrimeWrapper(firstModEntryWrapper, lastModEntryWrapper, temp);
    }
    return checkAllResidues(*primeWrapper, k);
}

bool tryProductsOfSmallPrimes(PrimeWrapper* primeWrapper, uint64_t residue1, ModEntry modEntry1) {
    if (primeWrapper == NULL) {
        if (modEntry1.modulus < SQRT_DIVBOUND) {
            fprintf(resultsDotTxt, "%"PRIu64"\n", modEntry1.modulus);
        }
        // base case. check for solution.
        return false;
    }
    
    if (tryProductsOfSmallPrimes(primeWrapper->prev, residue1, modEntry1)) {return true;}
    ModEntryWrapper* modEntryWrapper = primeWrapper->lastModEntryWrapper;
    ModEntry modEntry2, newModEntry;
    ResidueWrapper* residueWrapper;
    uint64_t newResidue;
    while (modEntryWrapper != NULL) {
        modEntry2 = modEntryWrapper->modEntry;
        residueWrapper = modEntryWrapper->residueHead;
        while (residueWrapper != NULL) {
            newModEntry = combineCoprimeModEntries(modEntry1, modEntry2);
            if (newModEntry.modulus > DIVBOUND) {return false;}
            newResidue = crtCalc(residue1, modEntry1, residueWrapper->residue, modEntry2, newModEntry);
            if (tryProductsOfSmallPrimes(primeWrapper->prev, newResidue, newModEntry)) {return true;}
            residueWrapper = residueWrapper->prev;
        }
        modEntryWrapper = modEntryWrapper->prev;
    }
    return false;
}

bool tryWithAllResidues(PrimeWrapper* primeWrapper, ModEntryWrapper modEntryWrapper2, uint64_t k) {
    ModEntryWrapper* modEntryWrapper1;
    ModEntry entry1, entry2 = modEntryWrapper2.modEntry, newEntry;
    ResidueWrapper* residueWrapper1,* residueWrapper2 = modEntryWrapper2.residueHead;
    uint64_t residue1, residue2, newResidue;
    while (residueWrapper2 != NULL) {
        residue2 = residueWrapper2->residue;
        if (checkResidueRunner(residue2, entry2.modulus, k)) {
            return true;
        }
        while (primeWrapper != NULL) {
            modEntryWrapper1 = primeWrapper->lastModEntryWrapper;
            while (modEntryWrapper1 != NULL) {
                entry1 = modEntryWrapper1->modEntry;
                newEntry = combineCoprimeModEntries(entry1, entry2);
                residueWrapper1 = modEntryWrapper1->residueHead;
                while (residueWrapper1 != NULL) {
                    residue1 = residueWrapper1->residue;
                    newResidue = crtCalc(residue1, entry1, residue2, entry2, newEntry);
                    if (checkResidueRunner(newResidue, newEntry.modulus, k)) {
                        return true;
                    }
                    residueWrapper2 = residueWrapper2->prev;
                    residueWrapper1 = residueWrapper1->prev;
                }
            modEntryWrapper1 = modEntryWrapper1->prev;
            }        
            primeWrapper = primeWrapper->prev;
        }
    }
    return false;
}

bool tryLargePrimes() {
    return false;
}

bool tryAdvanced(uint64_t k) {
    primesieve_iterator primeIterator;
    (void)primesieve_init(&primeIterator);
    PrimeWrapper* primeWrapperHead = NULL;
    bool result = false;
    const ModEntry ONE_ENTRY = makeModEntry(1, 18446744073709551615ULL, 0, 1);

    if (tryPowersOfSmallPrimes(k, &primeIterator, &primeWrapperHead)) {
        result = true;
    }
    else if (tryProductsOfSmallPrimes(primeWrapperHead, 0, ONE_ENTRY)){
        result = true;
    }
    else if (tryLargePrimes()) {
        result = true;
    }
    (void)freePrimeWrappers(primeWrapperHead);
    (void)primesieve_free_iterator(&primeIterator);
    return result;
}