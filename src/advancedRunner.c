#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minExp, int64_t maxExp) {
  const float ALPHA = 0.2599210498948734;
  int64_t bound, divbound;
  int64_t oldBound = 0;
  int64_t oldDivbound = 0;

  for (int64_t i = minExp; i <= maxExp; i++) {
    bound = (1LL << i) - 1;
    divbound = ALPHA * bound;
    if (tryAdvanced(k, bound, divbound, oldBound, oldDivbound)) {return true;}
    if (tryPreliminary(k, i, oldBound));
    oldBound = bound;
    oldDivbound = divbound;
  }
  return false;
}
