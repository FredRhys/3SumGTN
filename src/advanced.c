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

bool checkThisPrime(ModEntryWrapper* modEntryWrapper, uint64_t k) {
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

void extractRootsFromPower(ModEntryWrapper* modEntryWrapper, ResidueWrapper* residueWrapper, ModEntry primeEntry, uint64_t k) {
    const ModEntry MOD_ENTRY = modEntryWrapper->modEntry;
    const uint64_t _6k = montmul(6, k, MOD_ENTRY);
    uint64_t residue;
    while (residueWrapper != NULL) {
        residue = residueWrapper->residue;
        const uint64_t DENOMINATOR = applyMasterPolyDeriv(residue, MOD_ENTRY);
        //TODO figure out why this doesn't accept modulus of 4.
        if (DENOMINATOR != 0 && MOD_ENTRY.modulus != 4) {
            (void)extractRootsWithHensels(modEntryWrapper, primeEntry, residue, DENOMINATOR, _6k);
        }
        else {
            (void)extractRootsWithBruteForce(modEntryWrapper, _6k);
            break;
        }
        residueWrapper = residueWrapper->prev;
    }
}

bool checkSmallPowersOfThisPrime(uint64_t prime, uint64_t k, ModEntryWrapper** restrict firstPtr, ModEntryWrapper** restrict lastPtr, uint64_t sqrtDivbound) {
    ModEntry primeEntry = primeModEntry(prime);
    ModEntryWrapper first = makeModEntryWrapper(primeEntry, NULL);
    if (!checkThisPrime(&first, k)) {return false;}
    ResidueWrapper* residueWrapper = first.residueWrapper;
    *firstPtr = malloc(sizeof(ModEntryWrapper));
    **firstPtr = first;
    *lastPtr = malloc(sizeof(ModEntryWrapper));
    **lastPtr = first;
    ModEntryWrapper last;
    uint64_t modulus = prime * prime;
    ModEntry modEntry = increasePrimeModEntryPower(primeEntry, primeEntry);
    while (modulus < sqrtDivbound) {
        last = makeModEntryWrapper(modEntry, *lastPtr);
        (void)extractRootsFromPower(&last, residueWrapper, primeEntry, k);
        *lastPtr = malloc(sizeof(ModEntryWrapper));
        **lastPtr = last;
        modulus *= prime;
        modEntry = increasePrimeModEntryPower(modEntry, primeEntry);
        residueWrapper = last.residueWrapper;
    }
    return true;
}

static bool isSolution(uint64_t modulus, uint64_t k, uint64_t residue, int8_t SIGN, uint64_t divbound) {
    const int64_t INCREMENT = SIGN * modulus;
    for (int64_t z = residue; (SIGN) * z < divbound; z += INCREMENT) {
        if ((SIGN) * z > UINT42_MAX) {break;}
        __int128_t dividand = (__int128_t)z * z * z - z - 6 * k;
        if (checkFormulaResults(dividand, modulus, 3 * modulus, k, z)) {
            return true;
        }
    }
    return false;
}

bool isSolutionRunner(uint64_t residue, uint64_t modulus, uint64_t k, uint64_t divbound) {
    return isSolution(modulus, k, residue, 1, divbound) || isSolution(modulus, k, residue, -1, divbound);
}

bool checkAllResidues(PrimeWrapper* primeWrapper, uint64_t k, uint64_t divbound) {
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
            residueWrapper = modEntryWrapper->residueWrapper;
            while (residueWrapper != NULL) {
                residue = residueWrapper->residue;
                if (isSolutionRunner(residue, modulus, k, divbound)) {
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

bool checkSmallPowersOfSmallPrimes(uint64_t k, primesieve_iterator* primeIterator, PrimeWrapper** primeWrapper, uint64_t sqrtDivbound) {
    PrimeWrapper* temp;
    ModEntryWrapper* firstModEntryWrapper;
    ModEntryWrapper* lastModEntryWrapper;
    uint64_t prime;
    while ((prime = primesieve_next_prime(primeIterator)) < sqrtDivbound) {
        if (!checkSmallPowersOfThisPrime(prime, k, &firstModEntryWrapper, &lastModEntryWrapper, sqrtDivbound)) {continue;}        
        temp = *primeWrapper;
        *primeWrapper = malloc(sizeof(PrimeWrapper));
        **primeWrapper = makePrimeWrapper(firstModEntryWrapper, lastModEntryWrapper, temp);
    }
    return checkAllResidues(*primeWrapper, k, sqrtDivbound * sqrtDivbound);
}

bool tryBaseCRT(ModEntryWrapper** compositeWrapper, ModEntry modEntry, uint64_t residue, uint64_t k,uint64_t sqrtDivbound) {
    const uint64_t MODULUS = modEntry.modulus;
    if (isSolutionRunner(residue, MODULUS, k, sqrtDivbound * sqrtDivbound)) {return true;}
    if (MODULUS > sqrtDivbound) {return false;}
    if (*compositeWrapper != NULL) {
        ModEntry compositeEntry = (*compositeWrapper)->modEntry;
        if (compositeEntry.modulus == MODULUS) {
            goto tryBaseCRT_appendResidue;
        }
    }
    ModEntryWrapper* OLD = *compositeWrapper;
    *compositeWrapper = malloc(sizeof(ModEntryWrapper));
    **compositeWrapper = makeModEntryWrapper(modEntry, OLD);
tryBaseCRT_appendResidue:
    appendResidue(*compositeWrapper, residue);
    return false;
}

bool checkCRT(PrimeWrapper* primeWrapper, ModEntryWrapper** compositeWrapper, ModEntry inputModEntry, uint64_t inputResidue, uint64_t k, uint64_t sqrtDivbound);

bool crtSmallPowers(PrimeWrapper* primeWrapper, ModEntryWrapper** compositeWrapper, ModEntry inputModEntry, uint64_t inputResidue, uint64_t k, uint64_t sqrtDivbound) {
    ModEntryWrapper* modEntryIterator = primeWrapper->lastModEntryWrapper;
    ModEntry iteratedModEntry, newModEntry;
    ResidueWrapper* residueWrapper;
    const uint64_t DIVBOUND = sqrtDivbound * sqrtDivbound;
    uint64_t newResidue;
    while (modEntryIterator != NULL) {
        iteratedModEntry = modEntryIterator->modEntry;
        if (inputModEntry.modulus * iteratedModEntry.modulus < DIVBOUND) {
            newModEntry = combineCoprimeModEntries(inputModEntry, iteratedModEntry);
            residueWrapper = modEntryIterator->residueWrapper;
            while (residueWrapper != NULL) {
                newResidue = crtCalc(inputResidue, inputModEntry, residueWrapper->residue, iteratedModEntry, newModEntry);
                if (checkCRT(primeWrapper->prev, compositeWrapper, newModEntry, newResidue, k, sqrtDivbound)) {return true;}
                residueWrapper = residueWrapper->prev;
            }
        }
        modEntryIterator = modEntryIterator->prev;
    }
    return false;
}

bool crtLargePowers(PrimeWrapper* primeWrapper, ModEntryWrapper** compositeWrapper, ModEntry inputModEntry, uint64_t inputResidue, uint64_t k, uint64_t sqrtDivbound) {
    const ModEntryWrapper primeEntryWrapper = *(primeWrapper->firstModEntryWrapper);
    const ModEntry primeEntry = primeEntryWrapper.modEntry;
    ModEntryWrapper powerEntryWrapper = *(primeWrapper->lastModEntryWrapper);
    ResidueWrapper* residueWrapper = powerEntryWrapper.residueWrapper;
    ModEntry powerEntry = increasePrimeModEntryPower(powerEntryWrapper.modEntry, primeEntry);
    ModEntry newModEntry;
    const uint64_t INPUT_MODULUS = inputModEntry.modulus;
    const uint64_t DIVBOUND = sqrtDivbound * sqrtDivbound;
    uint64_t residue, newResidue;
    powerEntryWrapper = makeModEntryWrapper(powerEntry, NULL);
    bool freeing = false, result = false;
    while (powerEntry.modulus * INPUT_MODULUS < DIVBOUND) {
        newModEntry = combineCoprimeModEntries(powerEntry, inputModEntry);
        (void)extractRootsFromPower(&powerEntryWrapper, residueWrapper, primeEntry, k);
        if (freeing) {
            (void)freeResidueWrappers(residueWrapper);
        }
        residueWrapper = powerEntryWrapper.residueWrapper;
        while (residueWrapper != NULL) {
            residue = residueWrapper->residue;
            newResidue = crtCalc(residue, powerEntry, inputResidue, inputModEntry, newModEntry);
            if (checkCRT(primeWrapper, compositeWrapper, newModEntry, newResidue, k, sqrtDivbound)) {
                result = true;
                break;
            }
            residueWrapper = residueWrapper->prev;
        }
        residueWrapper = powerEntryWrapper.residueWrapper;
        powerEntry = increasePrimeModEntryPower(powerEntry, primeEntry);
        powerEntryWrapper = makeModEntryWrapper(powerEntry, &powerEntryWrapper);
        if (result) {break;}
        if (!freeing) {
            freeing = true;
        }
    }
    if (freeing) {
        (void)freeResidueWrappers(residueWrapper);
    }
    return result;
}

// Runs Chinese Remainder Theorem to iterate over all available composite moduli.
bool checkCRT(PrimeWrapper* primeWrapper, ModEntryWrapper** compositeWrapper, ModEntry inputModEntry, uint64_t inputResidue, uint64_t k, uint64_t sqrtDivbound) {
    if (primeWrapper == NULL) {
        return tryBaseCRT(compositeWrapper, inputModEntry, inputResidue, k, sqrtDivbound);
    }
    if (checkCRT(primeWrapper->prev, compositeWrapper, inputModEntry, inputResidue, k, sqrtDivbound)) {return true;}
    if (crtSmallPowers(primeWrapper, compositeWrapper, inputModEntry, inputResidue, k, sqrtDivbound)) {return true;}
    if (inputModEntry.modulus > sqrtDivbound) {return false;}
    if (crtLargePowers(primeWrapper, compositeWrapper, inputModEntry, inputResidue, k, sqrtDivbound)) {return true;}
    return false;
}

bool checkSmallComposites(PrimeWrapper* primeWrapper, ModEntryWrapper** compositeWrapper, uint64_t k, uint64_t sqrtDivbound) {
    const ModEntry ONE_ENTRY = makeModEntry(1, 18446744073709551615ULL, 0, 1); // precomputed values
    return checkCRT(primeWrapper, compositeWrapper, ONE_ENTRY, 0, k, sqrtDivbound);
}

bool checkLargeComposites(ModEntryWrapper* compositeWrapper, ModEntry inputModEntry, uint64_t inputResidue, uint64_t k, uint64_t divbound) {
    ModEntry iteratedModEntry, newModEntry;
    ResidueWrapper* residueWrapper;
    uint64_t iteratedModulus, iteratedResidue, newResidue;
    while (compositeWrapper != NULL) {
        iteratedModEntry = compositeWrapper->modEntry;
        iteratedModulus = iteratedModEntry.modulus;
        if (iteratedModulus * inputModEntry.modulus < divbound) {
            newModEntry = combineCoprimeModEntries(inputModEntry, iteratedModEntry);
            residueWrapper = compositeWrapper->residueWrapper;
            while (residueWrapper != NULL) {
                iteratedResidue = residueWrapper->residue;
                newResidue = crtCalc(inputResidue, inputModEntry, iteratedResidue, iteratedModEntry, newModEntry);
                if (isSolutionRunner(newResidue, newModEntry.modulus, k, divbound)) {return true;}
                residueWrapper = residueWrapper->prev;
            }
        }
        compositeWrapper = compositeWrapper->prev;
    }
    return false;
}

bool tryLargePrimes(ModEntryWrapper* compositeWrapper, uint64_t k, primesieve_iterator* primeIterator, uint64_t divbound) {
    ModEntryWrapper modEntryWrapper;
    ModEntry modEntry;
    ResidueWrapper* residueWrapper;
    bool result = false;
    uint64_t prime, residue;
    while ((prime = primesieve_next_prime(primeIterator)) < divbound) {
        modEntry = primeModEntry(prime);
        modEntryWrapper = makeModEntryWrapper(modEntry, NULL);
        if (!checkThisPrime(&modEntryWrapper, k)) {continue;}
        residueWrapper = modEntryWrapper.residueWrapper;
        while (residueWrapper != NULL) {
            residue = residueWrapper->residue;
            if (checkLargeComposites(compositeWrapper, modEntry, residue, k, divbound)) {
                result = true;
                break;
            }
            residueWrapper = residueWrapper->prev;
        }
        freeResidueWrappers(modEntryWrapper.residueWrapper);
        if (result) {break;}
    }
    return result;
}

bool tryAdvanced(uint64_t k, uint64_t sqrtDivbound) {
    primesieve_iterator primeIterator;
    (void)primesieve_init(&primeIterator);
    PrimeWrapper* primeWrapper = NULL;
    ModEntryWrapper* compositeWrapper = NULL;
    bool result = false;
    uint64_t DIVBOUND = sqrtDivbound * sqrtDivbound;

    if (checkSmallPowersOfSmallPrimes(k, &primeIterator, &primeWrapper, sqrtDivbound)) {
        result = true;
    }
    else if (checkSmallComposites(primeWrapper, &compositeWrapper, k, sqrtDivbound)) {
        result = true;
    }
    else if (tryLargePrimes(compositeWrapper, k, &primeIterator, DIVBOUND)) {
        result = true;
    }
    (void)freeModEntryWrappers(compositeWrapper);
    (void)freePrimeWrappers(primeWrapper);
    (void)primesieve_free_iterator(&primeIterator);
    return result;
}