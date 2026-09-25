#include "isQr.h"

bool isQrMod5(uint64_t operand) {
  switch (operand % 5) {
    case 0:
    case 1:
    case 4:
      return true;
    case 2:
    case 3:
      return false;
    default:
      return false;
  }
}

bool isQrMod7(uint64_t operand) {
  switch (operand % 7) {
    case 0:
    case 1:
    case 2:
    case 4:
      return true;
    case 3:
    case 5:
    case 6:
      return false;
    default:
      return false;
  }
}

bool isQrMod11(uint64_t operand) {
  switch (operand % 11) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 9:
      return true;
    case 2:
    case 6:
    case 7:
    case 8:
    case 10:
      return false;
    default:
      return false;
  }
}

bool isQrMod13(uint64_t operand) {
  switch (operand % 13) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 9:
    case 10:
    case 12:
      return true;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
      return false;
    default:
      return false;
  }
}

bool isQrModSmallPrimes(uint64_t operand) {
  if (!isQrMod5(operand)) {return false;}
  if (!isQrMod7(operand)) {return false;}
  if (!isQrMod11(operand)) {return false;}
  if (!isQrMod13(operand)) {return false;}
  return true;
}
