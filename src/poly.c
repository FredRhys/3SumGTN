#include "poly.h"

Poly makePoly(uint64_t deg2, uint64_t deg1, uint64_t deg0){
    return (Poly){deg2, deg1, deg0};
}

Poly mulPoly(Poly multiplier, Poly multiplicand, ModEntry modEntry, uint64_t int_6k) {
    const uint64_t multiplier2 = multiplier.deg2,
        multiplier1 = multiplier.deg1,
        multiplier0 = multiplier.deg0,
        multiplicand2 = multiplicand.deg2,
        multiplicand1 = multiplicand.deg1,
        multiplicand0 = multiplicand.deg0,
        modulus = modEntry.modulus;
    const uint64_t ALPHA = montmul(multiplier2, multiplicand2, modEntry),
        BETA = montmul(multiplier1, multiplicand1, modEntry),
        GAMMA = montmul(multiplier0, multiplicand0, modEntry),
        DELTA = montmul(int_6k, ALPHA, modEntry),
        EPSILON = montmul(addmod(multiplier2, multiplier1, modulus),
            addmod(multiplicand2, multiplicand1, modulus), modEntry),
        ZETA = montmul(addmod(multiplier1, multiplier0, modulus),
            addmod(multiplicand1, multiplicand0, modulus), modEntry),
        ETA = montmul(addmod(multiplier2, multiplier0, modulus),
            addmod(multiplicand2, multiplicand0, modulus), modEntry),
        THETA = submod(submod(EPSILON, ALPHA, modulus), BETA, modulus),
        IOTA = montmul(int_6k, THETA, modEntry);

        return makePoly(submod(ETA, submod(GAMMA, BETA, modulus), modulus),
            submod(submod(DELTA, submod(BETA, THETA, modulus), modulus),
                submod(GAMMA, ZETA, modulus), modulus),
            addmod(GAMMA, IOTA, modulus));

}

Poly mulPolyConst(Poly multiplier, uint64_t multiplicand, ModEntry modEntry) {
    return makePoly(montmul(multiplier.deg2, multiplicand, modEntry),
        montmul(multiplier.deg1, multiplicand, modEntry),
        montmul(multiplier.deg0, multiplicand, modEntry));
}

Poly getMonicPoly(Poly operand, ModEntry modEntry) {
    const uint64_t deg2 = operand.deg2;
    const uint64_t deg1 = operand.deg1;
    uint64_t multiplier = 1;
    if (deg2 > 1) {
        multiplier = invmod(deg2, modEntry);
    }
    else if (deg2 == 0 && deg1 > 1) {
        multiplier = invmod(deg1, modEntry);
    }
    return mulPolyConst(operand, multiplier, modEntry);
}

Poly getRootPoly(ModEntry modEntry, uint64_t int_6k, uint64_t offset) {
    const uint64_t prime = modEntry.modulus;
    uint64_t exponent = (prime-1)/2;
    Poly base = makePoly(0, 1, (offset == 0) ? 0 : prime - offset);
    Poly result = makePoly(0, 0, 1);

    while (exponent > 0) {
        if ((exponent & 0b1) == 1) {
            result = mulPoly(result, base, modEntry, int_6k);
        }
        base = mulPoly(base, base, modEntry, int_6k);
        exponent >>= 1;
    }
    result.deg0 = addmod(result.deg0, 1, prime);
    return result;
}

uint64_t applyMasterPoly(uint64_t operand, ModEntry modEntry, uint64_t int_6k) {
    const uint64_t modulus = modEntry.modulus;
    return submod(submod(montexp(operand, 3, modEntry), operand, modulus), int_6k, modulus);
}

uint64_t applyMasterPolyDeriv(uint64_t operand, ModEntry modEntry) {
    return submod(montmul(3, montexp(operand, 2, modEntry), modEntry), 1, modEntry.modulus);
}

uint8_t degreeOfPoly(Poly operand) {
    if (operand.deg2 != 0) {return 2;}
    if (operand.deg1 != 0) {return 1;}
    return 0;
}

uint8_t gcdPoly(Poly *result, Poly operand, ModEntry modEntry, uint64_t _6k) {
	const uint64_t PRIME = modEntry.modulus;
	operand = getMonicPoly(operand, modEntry);
	uint8_t degree = degreeOfPoly(operand);
	if (degree == 2) {
			const uint64_t deg1 = operand.deg1;
			const uint64_t deg0 = operand.deg0;
			const uint64_t denominator = submod(submod(montexp(deg1, 2, modEntry), deg0, PRIME), 1, PRIME);
			const uint64_t numerator = submod(montmul(deg1, deg0, modEntry), _6k, PRIME);
			if (denominator == 0) {
				if (numerator == 0) {
					*result = operand;
					return 2;
				}
				else {
					return 0;
				}
			}
			operand = makePoly(0, 1, montmul(numerator, invmod(denominator, modEntry), modEntry));
	}
	else if (degree == 0) {
		return 0;
	}
	const uint64_t deg0 = PRIME - operand.deg0;
	if (applyMasterPoly(deg0, modEntry, _6k) == 0) {
		*result = operand;
		return 1;
	}
	return 0;
}