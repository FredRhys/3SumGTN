#ifndef IS_QR_H
#define IS_QR_H

#include <inttypes.h>

/*

In writing this header file I collaborated with Claude to assist with optimization.
I affirm that all AI-generated and co-created content underwent thorough review and
evaluation. The final output actually reflects my understanding, expertise, and
intended meaning. While AI assistance was instrumental in the process, I maintain
full responsibility for the content, its accuracy, and its presentation. This
disclosure is made in the spirit of transparency and to acknowledge the role of AI
in the creation process.

I wrote an original implementation that used in-memory arrays of bools and
operations on 128-bit integers. Claude recognized these as two potential
slow-downs, which was consistent with the output of perf. Claude offered a solution
which I read through and made sure to understand, which I then accepted into the
project; see below.

N.B. the new method is not that which was described in the original paper.

*/

// Quadratic-residue test bitmasks: bit r is set iff r is a square (or 0) mod p.
// Mod 5: QR {0,1,4}; mod 7: {0,1,2,4}; mod 11: {0,1,3,4,5,9}; mod 13: {0,1,3,4,9,10,12}.
#define QR_MASK_5  0x13u //
#define QR_MASK_7  0x17u
#define QR_MASK_11 0x23Bu
#define QR_MASK_13 0x161Bu

// 5 * 7 * 11 * 13 = 5005. Reducing mod 5005 once lets all four tests use cheap 64-bit ops.
#define QR_PRODUCT 5005u
const static uint32_t QR_TWO_POW_64_MOD = (((__uint128_t)1 << 64) % QR_PRODUCT);

static inline bool isQrModSmallPrimes(__uint128_t operand) {
  const uint64_t hi = (uint64_t)(operand >> 64);
  const uint64_t lo = (uint64_t)operand;
  const uint32_t r = ((hi % QR_PRODUCT) * QR_TWO_POW_64_MOD + lo % QR_PRODUCT) % QR_PRODUCT;
  return ((QR_MASK_5 >> (r % 5)) & 1)
    && ((QR_MASK_7 >> (r % 7)) & 1)
    && ((QR_MASK_11 >> (r % 11)) & 1)
    && ((QR_MASK_13 >> (r % 13)) & 1);
}

#endif
