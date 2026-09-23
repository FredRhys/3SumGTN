#include "advancedRunner.h"

bool runAdvanced(uint64_t k, uint64_t minDivbound, uint64_t maxDivbound) {
  for (int64_t i = minDivbound; i <= maxDivbound; i *= 2) {
    if (tryAdvanced(k, i)) {return true;}
  }
  return false;
}
