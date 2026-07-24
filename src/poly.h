#ifndef POLY_H

#define POLY_H
#include "libmontmul/montmul.h"

typedef struct Poly{
    uint64_t deg2;
    uint64_t deg1;
    uint64_t deg0;
}Poly;

Poly makePoly(uint64_t deg2, uint64_t deg1, uint64_t deg0);
// Poly addPoly(Poly augend, Poly addend, uint64_t modulus);
// Poly subPoly(Poly minuend, Poly subtrahend, uint64_t modulus);
Poly mulPoly(Poly multiplier, Poly multiplicand, ModEntry modEntry, uint64_t int_6k);
Poly mulPolyConst(Poly multiplier, uint64_t multiplicand, ModEntry modEntry);
Poly getMonicPoly(Poly operand, ModEntry modEntry);
Poly getRootPoly(ModEntry modEntry, uint64_t int_6k, uint64_t offset);
uint64_t applyMasterPoly(uint64_t operand, ModEntry modEntry, uint64_t int_6k);
uint64_t applyMasterPolyDeriv(uint64_t operand, ModEntry modEntry);

#endif