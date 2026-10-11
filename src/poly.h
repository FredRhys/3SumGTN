#ifndef POLY_H

#define POLY_H
#include "../libmontmul/montmul.h"
#include <inttypes.h>
#include <stdbit.h>

typedef struct libmontmul_t libmontmul_t;

typedef struct Poly{
  uint64_t deg2;
  uint64_t deg1;
  uint64_t deg0;
}Poly;

Poly makePoly(uint64_t deg2, uint64_t deg1, uint64_t deg0);
Poly mulPoly(Poly multiplier, Poly multiplicand, libmontmul_t montmul_t, uint64_t int_6k);
Poly mulPolyConst(Poly multiplier, uint64_t multiplicand, libmontmul_t montmul_t);
Poly getMonicPoly(Poly operand, libmontmul_t montmul_t);
Poly getRootPoly(libmontmul_t montmul_t, uint64_t int_6k, uint64_t offset);
Poly gcdPoly(Poly operand, libmontmul_t montmul_t, uint64_t _6k);
uint64_t applyMasterPoly(uint64_t operand, libmontmul_t montmul_t, uint64_t int_6k);
uint64_t applyMasterPolyDeriv(uint64_t operand, libmontmul_t montmul_t);
uint8_t degreeOfPoly(Poly operand);

#endif
