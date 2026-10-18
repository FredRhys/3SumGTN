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
	const uint8_t bitWidth = myBitWidth(operand);
	const uint8_t excessBitWidth = bitWidth - 64;
	uint64_t root = (uint64_t)sqrtl((long double)operand);
	__uint128_t sq = (__uint128_t)root * root;
	if (bitWidth <= 64 || (operand & ((1LL << excessBitWidth)-1)) == 0) {
		if (sq == operand) {
			if (result != NULL) {*result = root;}
			return true;
		}
		return false;
	}
	const uint8_t exponent = ((excessBitWidth & 1) == 0) ? (excessBitWidth/2) : (excessBitWidth+1)/2;
	uint64_t high = root + (1LL<<exponent);
	uint64_t low = root - (1LL<<exponent);
	while (low <= high) {
		if (sq > operand) {
			high = root - 1;
		}
		else if (sq < operand) {
			low = root + 1;
		}
		else {
      if (result != NULL) {*result = root;}
			return true;
		}
		root = (high + low) / 2;
		sq = (__uint128_t)root * root;
	}
	return false;
}

__uint128_t abs128(__int128_t operand) {
	return operand >= 0 ? operand : -operand;
}

int64_t adjustSolutions(int64_t operand) {
	return (operand == 0 || operand == 1) ? 0 : operand + 1;
}

void fprintfResults(uint64_t x, uint64_t y, uint64_t z, uint64_t k) {
	z = adjustSolutions(z);
	x = adjustSolutions(x);
	y = adjustSolutions(y);
	(void)fprintf(resultsDotTxt, "%ld : %ld / %ld / %ld\n", k, x, y, z);
}

bool checkFormulaResults(__int128_t dividand, uint64_t divisor, libdivide_u64_t* tripleDivisor, uint64_t k, int64_t z) {
	__int128_t disc = 4 * (divisor+abs128(dividand)) - (__int128_t)divisor * divisor * divisor;
	if (disc < 0) {return false;}
	const uint64_t DIVISOR_TIMES_3 = divisor * 3;
	if (!isQrModSmallPrimes(disc * DIVISOR_TIMES_3)) {return false;}
	const int64_t RADICAND = disc <= UINT64_MAX ? libdivide_u64_do(disc, tripleDivisor) : disc / DIVISOR_TIMES_3;
	if (RADICAND * DIVISOR_TIMES_3 != disc) {return false;}
	if ((divisor & 0b1) != (RADICAND & 0b1)) {return false;}
	uint64_t formulaSqrt;
    if (!iSqrt128(RADICAND, &formulaSqrt)) {return false;}

	int64_t x = (divisor+formulaSqrt)/2;
	int64_t y = ((int64_t)divisor - (int64_t)formulaSqrt)/2;
	if (dividand > 0) {
		x = -x;
		y = -y;
	}
	(void)fprintfResults(x, y, z, k);	
	return true;
}
