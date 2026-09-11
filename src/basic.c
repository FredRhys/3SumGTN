#include "basic.h"

bool checkFormulaDividend(__int128_t dividend, uint64_t k, int64_t z) {
	int primeFactorCount, exponents[15];
	uint64_t primeFactors[15];
	const __uint128_t absDividend = abs128(dividend);
	if (absDividend > UINT64_MAX) {
		return false;
	}
	if (absDividend == 1) {
		return checkFormulaResults(1, 1, 3, k, z);
	}
	primeFactorCount = factor64(primeFactors, exponents, absDividend);
	uint64_t divisors[DIVISOR_LIM], nextDivisor, base, prime;
	divisors[0] = 1;
	uint32_t lastDivisorIndex = 0, multiplicandLimIndex;

	for (uint8_t primeIndex = 0; primeIndex < primeFactorCount; primeIndex++) {
		multiplicandLimIndex = lastDivisorIndex;
		prime = primeFactors[primeIndex];
		base = 1;
		for (int primePower = 1; primePower <= exponents[primeIndex]; primePower++) {
			base *= prime;
			for (uint32_t multiplicandIndex = 0; multiplicandIndex <= multiplicandLimIndex; multiplicandIndex++) {
				nextDivisor = base * divisors[multiplicandIndex];
				//if (nextDivisor > DIVBOUND) {continue;}
				if (checkFormulaResults(dividend, nextDivisor, 3*nextDivisor, k, z)) {return true;}
				if (lastDivisorIndex == DIVISOR_LIM - 1) {
					//printf("Filled divisiors.\n");
					return false;
				}
				divisors[++lastDivisorIndex] = nextDivisor;
			}
		}
	}
	return false;
}

bool tryBasic(uint64_t k) {
	bool result = false;
	int64_t negBound = -2 * sqrtl(k);
    int64_t _6k = 6 * k;
    __int128_t dividend = -1;
	for (int64_t z = negBound; dividend <= 0 && !result; z++) {
		dividend = (__int128_t)z * z * z - z - _6k;
		if (checkFormulaDividend(dividend, k, z)) {result = true;}
	}
	return result;

}