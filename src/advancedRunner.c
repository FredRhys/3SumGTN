#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minExp, int64_t maxExp) {
  const float ALPHA = 0.2599210498948734;
  bounds_t bounds = {0, 0, 0, 0, 0};
  int64_t bound, divbound;

  for (int64_t i = minExp; i <= maxExp; i++) {
    bound = (1LL << i);
    bounds.bound = bound;
    divbound = ALPHA * bound;
    bounds.divbound = divbound + 1;
    bounds.sqrtDivbound = sqrtf(divbound) + 1;
    if (tryAdvanced(k, bounds)) {return true;}
    if (tryPreliminary(k, bound, bounds.oldBound)) {return true;}
    bounds.oldBound = bounds.bound;
    bounds.oldDivbound = bounds.divbound;
  }
  return false;
}
