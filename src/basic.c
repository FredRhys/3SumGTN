#include "basic.h"

uint8_t myBitWidth(__uint128_t operand) {
    if (operand <= UINT64_MAX) {return stdc_bit_width_ull((uint64_t)operand);}
    return 64 + stdc_bit_width_ull(operand >> 64);
}

bool iSqrt128(__uint128_t operand, uint64_t *result) {
	if (operand <= 1) {
		if (result != NULL) {*result = operand;}
		return true;
	}
	const uint8_t m = myBitWidth(operand);
	uint64_t s = (uint64_t)sqrtl((long double)operand);
	__uint128_t sq = (__uint128_t)s * s;
	if (m <= 64) {
exact:
		if (sq == operand) {
			if (result != NULL) {*result = s;}
			return true;
		}
		return false;
	}
	const uint8_t n = m - 64;
	if ((operand & (n-1)) == 0) {goto exact;}
	const uint8_t exponent = ((n & 1) == 0) ? (n/2) : (n+1)/2;
	uint64_t high = s + (1<<exponent);
	uint64_t low = s - (1<<exponent);
	while (low <= high) {
		if (sq > operand) {
			high = s - 1;
		}
		else if (sq < operand) {
			low = s + 1;
		}
		else {
            if (result != NULL) {*result = s;}
			return true;
		}
		s = (high + low) / 2;
		sq = (__uint128_t)s * s;
	}
	return false;
}

__uint128_t abs128(__int128_t operand) {
	return operand >= 0 ? operand : -operand;
}

bool checkFormulaResults(__int128_t dividend, uint64_t divisor, uint64_t _3d) {
	__int128_t disc = 4 * (divisor+abs128(dividend)) - (__int128_t)divisor * divisor * divisor;
	int64_t sqrtand = disc/_3d;
	//uint64_t formulaSqrt;
	if (sqrtand * _3d != disc) {return false;}
	if ((divisor & 1) != (sqrtand & 1)) {return false;}
	//if (!checkModSmallPrimes(sqrtand, 1)) {return false;}
    return iSqrt128(sqrtand, NULL);

	//if (!iSqrt128(sqrtand, &formulaSqrt)) {return 0;}
	// z = (z==0 || z==1) ? 0 : z+1;
	// int64_t x = (divisor+formulaSqrt)/2;
	// int64_t y = ((int64_t)divisor - (int64_t)formulaSqrt)/2;
	// x *= (dividend > 0) ? 1 : -1;
	// y *= (dividend > 0) ? 1 : -1;
	// x = (x==0 || x==1) ? 0 : x+1;
	// y = (y==0 || y==1) ? 0 : y+1;
	// FILE* f = fopen("reps.txt", "a");
	// fprintf(f, "%ld & %ld & %ld & %ld\n", int_6k / 6, x, y, z);
	// fclose(f);
	// if (abs128(z) >= abs128(biggest_z)) {
	// 	biggest_z = z;
	// 	biggest_y = y;
	// 	biggest_x = x;
	// }
	//return 1;
}

bool checkFormulaDividend(__int128_t dividend) {
	int primeFactorCount, exponents[15];
	uint64_t primeFactors[15];
	const __uint128_t absDividend = abs128(dividend);
	if (absDividend > UINT64_MAX) {
		return false;
	}
	if (absDividend == 1) {
		return checkFormulaResults(1, 1, 3);
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
				if (checkFormulaResults(dividend, nextDivisor, 3*nextDivisor)) {return true;}
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
	if (initfactor64("factor64/factor.bin") < 0) {
		fprintf(stderr, "Cannot read factor data\n");
		return -1;
	}
	bool result = false;
	int64_t negBound = -2 * sqrtl(k);
    int64_t _6k = 6 * k;
    __int128_t dividend = -1;
	for (int64_t z = negBound; dividend <= 0 && !result; z++) {
		dividend = (__int128_t)z * z * z - z - _6k;
		if (checkFormulaDividend(dividend)) {result = true;}
	}
	return result;

}