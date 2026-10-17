#include "advanced.h"

bool extractRootsFromQuad(Poly poly, ModulusWrapper* modulusWrapper) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t PRIME = MONTMUL_T.modulus;
  const uint64_t A = poly.deg2, B = poly.deg1, C = poly.deg0;
  const uint64_t DENOMINATOR = invmod(montmul(2, A, MONTMUL_T), MONTMUL_T);
  const uint64_t DISCRIMINANT = submod(montexp(B, 2, MONTMUL_T), montmul(4, montmul(A, C, MONTMUL_T), MONTMUL_T), PRIME);
  if (legendre(DISCRIMINANT, MONTMUL_T) == -1) {return false;}
  const uint64_t SQRT = sqrtmod(DISCRIMINANT, MONTMUL_T);
  const uint64_t NUMERATOR1 = submod(SQRT, B, PRIME);
  const uint64_t NUMERATOR2 = submod(0, B + SQRT, PRIME);
  const uint64_t ROOT1 = montmul(NUMERATOR1, DENOMINATOR, MONTMUL_T);
  appendResidue(modulusWrapper, ROOT1);
  if(NUMERATOR1 == NUMERATOR2) {return true;}
  const uint64_t ROOT2 = montmul(NUMERATOR2, DENOMINATOR, MONTMUL_T);
  appendResidue(modulusWrapper, ROOT2);
  return true;
}

bool extractRoots(Poly poly, ModulusWrapper* modulusWrapper) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t PRIME = MONTMUL_T.modulus;
  switch (degreeOfPoly(poly)) {
    case 2:
      return extractRootsFromQuad(poly, modulusWrapper);
    case 1:
      uint64_t root = submod(0, poly.deg0, PRIME);
      appendResidue(modulusWrapper, root);
      return true;
    default:
      return false;
  }

}

void checkOffsetIsRoot(ModulusWrapper* modulusWrapper, uint64_t _6k, uint64_t offset, uint8_t degreeSum) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  switch (degreeSum) {
    case 2:
      appendResidue(modulusWrapper, offset);
      break;
    case 0:
      breakif(applyMasterPoly(offset, MONTMUL_T, _6k) != 0);
      appendResidue(modulusWrapper, offset);
      break;
    default:
      break;
  }
}

bool extractRootsFromPrime(ModulusWrapper* modulusWrapper, uint64_t k) {
  Poly rootPoly;
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t PRIME = MONTMUL_T.modulus;
  const uint64_t _6k = montmul(6, k, MONTMUL_T);
  for (uint64_t offset = 0; offset < PRIME; offset++) {
    rootPoly = getRootPoly(MONTMUL_T, _6k, offset);
    continueif(degreeOfPoly(rootPoly) == 0);
    const Poly GCD1 = gcdPoly(rootPoly, MONTMUL_T, _6k);
    rootPoly.deg0 = submod(rootPoly.deg0, 2, PRIME);
    const Poly GCD2 = gcdPoly(rootPoly, MONTMUL_T, _6k);
    (void)checkOffsetIsRoot(modulusWrapper, _6k, offset, degreeOfPoly(GCD1) + degreeOfPoly(GCD2));
    const bool RESULT1 = extractRoots(GCD1, modulusWrapper);
    return extractRoots(GCD2, modulusWrapper) || RESULT1;
  }
  return false;
}

bool checkThisPrime(ModulusWrapper* modulusWrapper, uint64_t k) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t PRIME = MONTMUL_T.modulus;
  switch (PRIME) {
    case 3:
      appendResidue(modulusWrapper, 2);
      // Employing intentional case fallthrough
    case 2:
      appendResidue(modulusWrapper, 1);
      appendResidue(modulusWrapper, 0);
      return true;
    default:
      return extractRootsFromPrime(modulusWrapper, k);
  }  
}

void extractRootsWithHensels(ModulusWrapper* modulusWrapper, libmontmul_t primeMontmul_t, uint64_t oldRoot, uint64_t denominator, uint64_t _6k) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t MODULUS = MONTMUL_T.modulus;
  const uint64_t NEW_ROOT = submod(oldRoot,
    montmul(invmod(denominator, primeMontmul_t), applyMasterPoly(oldRoot, MONTMUL_T, _6k), MONTMUL_T),
    MODULUS);
  appendResidue(modulusWrapper, NEW_ROOT);
}

void extractRootsWithBruteForce(ModulusWrapper* modulusWrapper, uint64_t _6k) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t MODULUS = MONTMUL_T.modulus;
  // TODO can determine number of roots with implemented tools
  for (uint64_t i = 0; i < MODULUS; i++) {
    continueif(applyMasterPoly(i, MONTMUL_T, _6k) != 0);
    appendResidue(modulusWrapper, i);
  }
}

void extractRootsFromPower(ModulusWrapper* modulusWrapper, ResidueWrapper* residueWrapper, libmontmul_t primeMontmul_t, uint64_t k) {
  const libmontmul_t MONTMUL_T = modulusWrapper->modulus;
  const uint64_t _6k = montmul(6, k, MONTMUL_T);
  uint64_t residue;
  while (residueWrapper != NULL) {
    residue = residueWrapper->residue;
    const uint64_t DENOMINATOR = applyMasterPolyDeriv(residue, MONTMUL_T);
    //TODO figure out why this doesn't accept modulus of 4.
    if (DENOMINATOR != 0 && MONTMUL_T.modulus != 4) {
      (void)extractRootsWithHensels(modulusWrapper, primeMontmul_t, residue, DENOMINATOR, _6k);
    }
    else {
      (void)extractRootsWithBruteForce(modulusWrapper, _6k);
      break;
    }
    residueWrapper = residueWrapper->prev;
  }
}

bool checkSmallPowersOfThisPrime(uint64_t prime, uint64_t k, ModulusWrapper** restrict firstPtr, ModulusWrapper** restrict lastPtr, bounds_t bounds) {
  libmontmul_t primeMontmul_t = primeLibmontmul_t(prime);
  ModulusWrapper first = makeModulusWrapper(primeMontmul_t, NULL);
  if (!checkThisPrime(&first, k)) {return false;}
  ResidueWrapper* residueWrapper = first.residueWrapper;
  *firstPtr = malloc(sizeof(ModulusWrapper));
  **firstPtr = first;
  *lastPtr = malloc(sizeof(ModulusWrapper));
  **lastPtr = first;
  ModulusWrapper last;
  uint64_t modulus = prime * prime;
  libmontmul_t montmul_t = powerLibmontmul_t(primeMontmul_t, primeMontmul_t);
  const int64_t SQRT_DIVBOUND = bounds.sqrtDivbound;
  while (modulus < SQRT_DIVBOUND) {
    last = makeModulusWrapper(montmul_t, *lastPtr);
    (void)extractRootsFromPower(&last, residueWrapper, primeMontmul_t, k);
    *lastPtr = malloc(sizeof(ModulusWrapper));
    **lastPtr = last;
    modulus *= prime;
    montmul_t = powerLibmontmul_t(montmul_t, primeMontmul_t);
    residueWrapper = last.residueWrapper;
  }
  return true;
}

static inline bool isAbsLessThan(int64_t operand, int64_t boundary) {
  return operand > -boundary && operand < boundary;
}

static bool isSolution(uint64_t modulus, uint64_t k, uint64_t residue, int8_t SIGN, bounds_t bounds) {
  const int64_t INCREMENT = SIGN * modulus;
  const int64_t BOUND = bounds.bound;
  const int64_t OLD_BOUND = bounds.oldBound;
  const bool IS_MODULUS_SMALL = modulus < bounds.oldDivbound;
  libdivide_u64_t tripleDivisor = libdivide_u64_gen(3 * modulus);
  for (int64_t z = residue; isAbsLessThan(z, BOUND); z += INCREMENT) {
    /*
      VERY STRANGE BUG
      this will not solve 10873 : -426 / 411 / 209 when
      the minBound = 2^10
      */
    continueif(IS_MODULUS_SMALL && isAbsLessThan(z, OLD_BOUND));
    __int128_t dividand = (__int128_t)z * z * z - z - 6 * k;
    if (checkFormulaResults(dividand, modulus, &tripleDivisor, k, z)) {return true;}
  }
  return false;
}

bool isSolutionRunner(uint64_t residue, uint64_t modulus, uint64_t k, bounds_t bounds) {
  return isSolution(modulus, k, residue, 1, bounds) || isSolution(modulus, k, residue, -1, bounds);
}

bool checkAllResidues(PrimeWrapper* primeWrapper, uint64_t k, bounds_t bounds) {
  ModulusWrapper* modulusWrapper;
  libmontmul_t montmul_t;
  ResidueWrapper* residueWrapper;
  uint64_t modulus;
  uint64_t residue;
  while (primeWrapper != NULL) {
    modulusWrapper = primeWrapper->last;
    while (modulusWrapper != NULL) {
      montmul_t = modulusWrapper->modulus;
      modulus = montmul_t.modulus;
      residueWrapper = modulusWrapper->residueWrapper;
      while (residueWrapper != NULL) {
        residue = residueWrapper->residue;
        if (isSolutionRunner(residue, modulus, k, bounds)) {return true;}
        residueWrapper = residueWrapper->prev;
      }
      modulusWrapper = modulusWrapper->prev;
    }
    
    primeWrapper = primeWrapper->prev;
  }
  return false;
}

bool checkSmallPowersOfSmallPrimes(uint64_t k, primesieve_iterator* primeIterator, PrimeWrapper** primeWrapper, bounds_t bounds) {
  PrimeWrapper* temp;
  ModulusWrapper* firstMontmul_tWrapper;
  ModulusWrapper* lastMontmul_tWrapper;
  uint64_t prime;
  const int64_t SQRT_DIVBOUND = bounds.sqrtDivbound;
  while ((prime = primesieve_next_prime(primeIterator)) < SQRT_DIVBOUND) {
    continueif(!checkSmallPowersOfThisPrime(prime, k, &firstMontmul_tWrapper, &lastMontmul_tWrapper, bounds));
    temp = *primeWrapper;
    *primeWrapper = malloc(sizeof(PrimeWrapper));
    **primeWrapper = makePrimeWrapper(firstMontmul_tWrapper, lastMontmul_tWrapper, temp);
  }
  return checkAllResidues(*primeWrapper, k, bounds);
}

bool tryBaseCRT(ModulusWrapper** compositeWrapper, libmontmul_t montmul_t, uint64_t residue, uint64_t k, bounds_t bounds) {
  const uint64_t MODULUS = montmul_t.modulus;
  if (isSolutionRunner(residue, MODULUS, k, bounds)) {return true;}
  if (MODULUS >  bounds.sqrtDivbound) {return false;}
  if (*compositeWrapper != NULL) {
    ModulusWrapper* modulusWrapperIterator = *compositeWrapper;
    while (modulusWrapperIterator != NULL) {
      const libmontmul_t compositeEntry = modulusWrapperIterator->modulus;
      if (compositeEntry.modulus == MODULUS) {
        appendResidue(modulusWrapperIterator, residue);
        return false;
      }
      modulusWrapperIterator = modulusWrapperIterator->prev;
    }
  }
  // Makes a new mod entry wrapperwrapper
  ModulusWrapper* OLD = *compositeWrapper;
  *compositeWrapper = malloc(sizeof(ModulusWrapper));
  **compositeWrapper = makeModulusWrapper(montmul_t, OLD);
  appendResidue(*compositeWrapper, residue);
  return false;
}

bool checkCRT(PrimeWrapper* primeWrapper, ModulusWrapper** compositeWrapper, libmontmul_t inputMontmul_t, uint64_t inputResidue, uint64_t k, bounds_t bounds);

bool crtSmallPowers(PrimeWrapper* primeWrapper, ModulusWrapper** compositeWrapper, libmontmul_t inputMontmul_t, uint64_t inputResidue, uint64_t k, bounds_t bounds) {
  ModulusWrapper* montmul_tIterator = primeWrapper->last;
  libmontmul_t iteratedMontmul_t, newMontmul_t;
  ResidueWrapper* residueWrapper;
  const int64_t DIVBOUND = bounds.divbound;
  uint64_t newResidue;
  while (montmul_tIterator != NULL) {
    iteratedMontmul_t = montmul_tIterator->modulus;
    if (inputMontmul_t.modulus * iteratedMontmul_t.modulus < DIVBOUND) {
      newMontmul_t = compositeLibmontmul_t(inputMontmul_t, iteratedMontmul_t);
      residueWrapper = montmul_tIterator->residueWrapper;
      while (residueWrapper != NULL) {
        newResidue = crtCalc(inputResidue, inputMontmul_t, residueWrapper->residue, iteratedMontmul_t, newMontmul_t);
        if (checkCRT(primeWrapper->prev, compositeWrapper, newMontmul_t, newResidue, k, bounds)) {return true;}
        residueWrapper = residueWrapper->prev;
      }
    }
    montmul_tIterator = montmul_tIterator->prev;
  }
  return false;
}

bool crtLargePowers(PrimeWrapper* primeWrapper, ModulusWrapper** compositeWrapper, libmontmul_t inputMontmul_t, uint64_t inputResidue, uint64_t k, bounds_t bounds) {
  const ModulusWrapper primeMontmul_tWrapper = *(primeWrapper->first);
  const libmontmul_t primeMontmul_t = primeMontmul_tWrapper.modulus;
  ModulusWrapper powerEntryWrapper = *(primeWrapper->last);
  ResidueWrapper* residueWrapper = powerEntryWrapper.residueWrapper;
  libmontmul_t powerEntry = powerLibmontmul_t(powerEntryWrapper.modulus, primeMontmul_t);
  libmontmul_t newMontmul_t;
  const uint64_t INPUT_MODULUS = inputMontmul_t.modulus;
  const int64_t DIVBOUND = bounds.divbound;
  uint64_t residue, newResidue;
  powerEntryWrapper = makeModulusWrapper(powerEntry, NULL);
  bool freeing = false, result = false;
  while (powerEntry.modulus * INPUT_MODULUS < DIVBOUND) {
    newMontmul_t = compositeLibmontmul_t(powerEntry, inputMontmul_t);
    (void)extractRootsFromPower(&powerEntryWrapper, residueWrapper, primeMontmul_t, k);
    if (freeing) {
      (void)freeResidueWrappers(residueWrapper);
    }
    residueWrapper = powerEntryWrapper.residueWrapper;
    while (residueWrapper != NULL) {
      residue = residueWrapper->residue;
      newResidue = crtCalc(residue, powerEntry, inputResidue, inputMontmul_t, newMontmul_t);
      if (checkCRT(primeWrapper, compositeWrapper, newMontmul_t, newResidue, k, bounds)) {
        result = true;
        break;
      }
      residueWrapper = residueWrapper->prev;
    }
    residueWrapper = powerEntryWrapper.residueWrapper;
    powerEntry = powerLibmontmul_t(powerEntry, primeMontmul_t);
    powerEntryWrapper = makeModulusWrapper(powerEntry, &powerEntryWrapper);
    breakif(result);
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
bool checkCRT(PrimeWrapper* primeWrapper, ModulusWrapper** compositeWrapper, libmontmul_t inputMontmul_t, uint64_t inputResidue, uint64_t k, bounds_t bounds) {
  if (primeWrapper == NULL) {
    return tryBaseCRT(compositeWrapper, inputMontmul_t, inputResidue, k, bounds);
  }
  if (checkCRT(primeWrapper->prev, compositeWrapper, inputMontmul_t, inputResidue, k, bounds)) {return true;}
  if (crtSmallPowers(primeWrapper, compositeWrapper, inputMontmul_t, inputResidue, k, bounds)) {return true;}
  if (inputMontmul_t.modulus > bounds.sqrtDivbound) {return false;}
  if (crtLargePowers(primeWrapper, compositeWrapper, inputMontmul_t, inputResidue, k, bounds)) {return true;}
  return false;
}

bool checkSmallComposites(PrimeWrapper* primeWrapper, ModulusWrapper** compositeWrapper, uint64_t k, bounds_t bounds) {
  const libmontmul_t ONE = makeLibmontmul_t(1, 18446744073709551615ULL, 0, 1); // precomputed values
  return checkCRT(primeWrapper, compositeWrapper, ONE, 0, k, bounds);
}

bool checkLargeComposites(ModulusWrapper* compositeWrapper, libmontmul_t inputMontmul_t, uint64_t inputResidue, uint64_t k, bounds_t bounds) {
  libmontmul_t iteratedMontmul_t, newMontmul_t;
  ResidueWrapper* residueWrapper;
  uint64_t iteratedModulus, iteratedResidue, newResidue;
  const int64_t DIVBOUND = bounds.divbound;
  const int64_t INPUT_MODULUS = inputMontmul_t.modulus;
  while (compositeWrapper != NULL) {
    iteratedMontmul_t = compositeWrapper->modulus;
    iteratedModulus = iteratedMontmul_t.modulus;
    if (iteratedModulus * INPUT_MODULUS < DIVBOUND) {
      newMontmul_t = compositeLibmontmul_t(inputMontmul_t, iteratedMontmul_t);
      residueWrapper = compositeWrapper->residueWrapper;
      while (residueWrapper != NULL) {
        iteratedResidue = residueWrapper->residue;
        newResidue = crtCalc(inputResidue, inputMontmul_t, iteratedResidue, iteratedMontmul_t, newMontmul_t);
        if (isSolutionRunner(newResidue, newMontmul_t.modulus, k, bounds)) {return true;}
        residueWrapper = residueWrapper->prev;
      }
    }
    compositeWrapper = compositeWrapper->prev;
  }
  return false;
}

bool tryLargePrimes(ModulusWrapper* compositeWrapper, uint64_t k, primesieve_iterator* primeIterator, bounds_t bounds) {
  ModulusWrapper modulusWrapper;
  libmontmul_t montmul_t;
  ResidueWrapper* residueWrapper;
  bool result = false;
  uint64_t prime, residue;
  const int64_t DIVBOUND = bounds.divbound;
  while ((prime = primesieve_next_prime(primeIterator)) < DIVBOUND) {
    montmul_t = primeLibmontmul_t(prime);
    modulusWrapper = makeModulusWrapper(montmul_t, NULL);
    continueif(!checkThisPrime(&modulusWrapper, k));
    residueWrapper = modulusWrapper.residueWrapper;
    while (residueWrapper != NULL) {
      residue = residueWrapper->residue;
      if (checkLargeComposites(compositeWrapper, montmul_t, residue, k, bounds)) {
        result = true;
        break;
      }
      residueWrapper = residueWrapper->prev;
    }
    freeResidueWrappers(modulusWrapper.residueWrapper);
    breakif(result);
  }
  return result;
}

bool tryAdvanced(uint64_t k, bounds_t bounds) {
  primesieve_iterator primeIterator;
  (void)primesieve_init(&primeIterator);
  PrimeWrapper* primeWrapper = NULL;
  ModulusWrapper* compositeWrapper = NULL;
  bool result = false;

  if (checkSmallPowersOfSmallPrimes(k, &primeIterator, &primeWrapper, bounds)) {
    result = true;
  }
  else if (checkSmallComposites(primeWrapper, &compositeWrapper, k, bounds)) {
    result = true;
  }
  else if (tryLargePrimes(compositeWrapper, k, &primeIterator, bounds)) {
    result = true;
  }
  (void)freePrimeWrappers(primeWrapper);
  (void)freeModulusWrappers(compositeWrapper);
  (void)primesieve_free_iterator(&primeIterator);
  return result;
}
