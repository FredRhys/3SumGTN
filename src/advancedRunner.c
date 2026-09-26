#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minDivbound, int64_t maxDivbound) {
  int64_t oldDivbound = 0;
  for (int64_t i = minDivbound; i <= maxDivbound; i += minDivbound) {
    if (tryAdvanced(k, i, oldDivbound)) {return true;}
    oldDivbound = i * i;
  }
  return false;
}
