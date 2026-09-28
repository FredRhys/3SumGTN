#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minBound, int64_t maxBound) {
  const float ALPHA = 0.2599210498948734;
  int64_t divbound;
  int64_t oldBound = 0;
  int64_t oldDivbound = 0;

  for (int64_t i = minBound; i <= maxBound; i *= 2) {
    divbound = ALPHA * i;
    if (tryAdvanced(k, i, divbound, oldBound, oldDivbound)) {return true;}
    if (tryPreliminary(k, i, oldBound));
    oldBound = i;
    oldDivbound = divbound;
  }
  return false;
}
