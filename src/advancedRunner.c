#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minExp, int64_t maxExp) {
  const float ALPHA = 0.2599210498948734;
  int64_t bound = 1LL << minExp, divbound, oldBound = 0, oldDivbound = 0;
  for (int64_t i = minExp; i <= maxExp; i++) {
    divbound = ALPHA * bound;
    const bounds_t bounds = {bound + 1, divbound + 1, sqrtf(divbound) + 1, oldBound, oldDivbound};
    if (tryAdvanced(k, bounds)) {return true;}
    if (tryPreliminary(k, bound, oldBound)) {return true;}
    oldBound = bounds.bound;
    oldDivbound = bounds.divbound;
    bound <<= 1;
  }
  return false;
}
