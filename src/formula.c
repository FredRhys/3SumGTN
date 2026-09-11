#include "formula.h"

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

int64_t adjustSolutions(int64_t operand) {
	return (operand == 0 || operand == 1) ? 0 : operand + 1;
}

bool checkFormulaResults(__int128_t dividend, uint64_t divisor, uint64_t _3d, uint64_t k, int64_t z) {
	__int128_t disc = 4 * (divisor+abs128(dividend)) - (__int128_t)divisor * divisor * divisor;
	int64_t sqrtand = disc/_3d;
	if (sqrtand * _3d != disc) {return false;}
	if ((divisor & 1) != (sqrtand & 1)) {return false;}
	uint64_t formulaSqrt;
    if (!iSqrt128(sqrtand, &formulaSqrt)) {return false;}

	// printing results
	z = adjustSolutions(z);
	int64_t x = (divisor+formulaSqrt)/2;
	int64_t y = ((int64_t)divisor - (int64_t)formulaSqrt)/2;
	if (dividend > 0) {
		x = -x;
		y = -y;
	}
	x = adjustSolutions(x);
	y = adjustSolutions(y);
	(void)fprintf(resultsDotTxt, "%ld : %ld / %ld / %ld\n", k, x, y, z);
	return true;
}