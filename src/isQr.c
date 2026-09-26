#include "isQr.h"

bool isQrMod5(__uint128_t operand) {
  switch (operand % 5) {
    case 0:
    case 1:
    case 4:
      return true;
    default:
      return false;
  }
}

bool isQrMod7(__uint128_t operand) {
  switch (operand % 7) {
    case 0:
    case 1:
    case 2:
    case 4:
      return true;
    default:
      return false;
  }
}

bool isQrMod11(__uint128_t operand) {
  switch (operand % 11) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 9:
      return true;
    default:
      return false;
  }
}

bool isQrMod13(__uint128_t operand) {
  switch (operand % 13) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 9:
    case 10:
    case 12:
      return true;
    default:
      return false;
  }
}

bool isQrModSmallPrimes(__uint128_t operand) {
  if (!isQrMod5(operand)) {return false;}
  if (!isQrMod7(operand)) {return false;}
  if (!isQrMod11(operand)) {return false;}
  if (!isQrMod13(operand)) {return false;}
  return true;
}
