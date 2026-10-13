#include "poly.h"

Poly makePoly(uint64_t deg2, uint64_t deg1, uint64_t deg0){
  return (Poly){deg2, deg1, deg0};
}

Poly mulPoly(Poly multiplier, Poly multiplicand, libmontmul_t montmul_t, uint64_t int_6k) {
  const uint64_t multiplier2 = multiplier.deg2,
    multiplier1 = multiplier.deg1,
    multiplier0 = multiplier.deg0,
    multiplicand2 = multiplicand.deg2,
    multiplicand1 = multiplicand.deg1,
    multiplicand0 = multiplicand.deg0,
    modulus = montmul_t.modulus;
  const uint64_t ALPHA = montmul(multiplier2, multiplicand2, montmul_t),
    BETA = montmul(multiplier1, multiplicand1, montmul_t),
    GAMMA = montmul(multiplier0, multiplicand0, montmul_t),
    DELTA = montmul(int_6k, ALPHA, montmul_t),
    EPSILON = montmul(addmod(multiplier2, multiplier1, modulus),
      addmod(multiplicand2, multiplicand1, modulus), montmul_t),
    ZETA = montmul(addmod(multiplier1, multiplier0, modulus),
      addmod(multiplicand1, multiplicand0, modulus), montmul_t),
    ETA = montmul(addmod(multiplier2, multiplier0, modulus),
      addmod(multiplicand2, multiplicand0, modulus), montmul_t),
    THETA = submod(submod(EPSILON, ALPHA, modulus), BETA, modulus),
    IOTA = montmul(int_6k, THETA, montmul_t);

    return makePoly(submod(ETA, submod(GAMMA, BETA, modulus), modulus),
      submod(submod(DELTA, submod(BETA, THETA, modulus), modulus),
        submod(GAMMA, ZETA, modulus), modulus),
      addmod(GAMMA, IOTA, modulus));

}

Poly mulPolyConst(Poly multiplier, uint64_t multiplicand, libmontmul_t montmul_t) {
  return makePoly(montmul(multiplier.deg2, multiplicand, montmul_t),
    montmul(multiplier.deg1, multiplicand, montmul_t),
    montmul(multiplier.deg0, multiplicand, montmul_t));
}

Poly getMonicPoly(Poly operand, libmontmul_t montmul_t) {
  const uint64_t deg2 = operand.deg2;
  const uint64_t deg1 = operand.deg1;
  uint64_t multiplier = 1;
  if (deg2 > 1) {
    multiplier = invmod(deg2, montmul_t);
  }
  else if (deg2 == 0 && deg1 > 1) {
    multiplier = invmod(deg1, montmul_t);
  }
  return mulPolyConst(operand, multiplier, montmul_t);
}

Poly getRootPoly(libmontmul_t montmul_t, uint64_t int_6k, uint64_t offset) {
  const uint64_t PRIME = montmul_t.modulus;
  uint64_t exponent = (PRIME-1)/2;
  Poly base = makePoly(0, 1, submod(0, offset, PRIME));
  Poly result = makePoly(0, 0, 1);

  while (exponent > 0) {
    if ((exponent & 0b1) == 1) {
      result = mulPoly(result, base, montmul_t, int_6k);
    }
    base = mulPoly(base, base, montmul_t, int_6k);
    exponent >>= 1;
  }
  result.deg0 = addmod(result.deg0, 1, PRIME);
  return result;
}

uint64_t applyMasterPoly(uint64_t operand, libmontmul_t montmul_t, uint64_t int_6k) {
  const uint64_t modulus = montmul_t.modulus;
  return submod(submod(montexp(operand, 3, montmul_t), operand, modulus), int_6k, modulus);
}

uint64_t applyMasterPolyDeriv(uint64_t operand, libmontmul_t montmul_t) {
  return submod(montmul(3, montexp(operand, 2, montmul_t), montmul_t), 1, montmul_t.modulus);
}

uint8_t degreeOfPoly(Poly operand) {
  if (operand.deg2 != 0) {return 2;}
  if (operand.deg1 != 0) {return 1;}
  return 0;
}

Poly gcdPoly(Poly operand, libmontmul_t montmul_t, uint64_t _6k) {
	const uint64_t PRIME = montmul_t.modulus;
	operand = getMonicPoly(operand, montmul_t);
	uint8_t degree = degreeOfPoly(operand);
	if (degree == 2) {
			const uint64_t deg1 = operand.deg1;
			const uint64_t deg0 = operand.deg0;
			const uint64_t denominator = submod(submod(montexp(deg1, 2, montmul_t), deg0, PRIME), 1, PRIME);
			const uint64_t numerator = submod(montmul(deg1, deg0, montmul_t), _6k, PRIME);
			if (denominator == 0) {
				if (numerator == 0) {
					return operand;
				}
        goto fail;
			}
			operand = makePoly(0, 1, montmul(numerator, invmod(denominator, montmul_t), montmul_t));
	}
	else if (degree == 0) {
		goto fail;
	}
	const uint64_t deg0 = PRIME - operand.deg0;
	if (applyMasterPoly(deg0, montmul_t, _6k) == 0) {
		return operand;
	}
  fail:
	return makePoly(0, 0, 0);
}