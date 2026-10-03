#ifndef IS_QR_H
#define IS_QR_H

#include <inttypes.h>
#include <stdbool.h>

#include "isQr.h"

static bool isQrMod5[5] = {true, true, false, false, true};
static bool isQrMod7[7] = {true, true, true, false, true, false, false};
static bool isQrMod11[11] = {true, true, false, true, true, true, false, false, false, true};
static bool isQrMod13[13] = {true, true, false, true, true, false, false, false, false, true, true, false, true};

static inline bool isQrModSmallPrimes(__uint128_t operand) {
  return isQrMod5[operand % 5]
    && isQrMod7[operand % 7]
    && isQrMod11[operand % 11]
    && isQrMod13[operand % 13];
}

#endif
