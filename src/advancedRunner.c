#include "advancedRunner.h"

bool runAdvanced(uint64_t k, int64_t minBound, int64_t maxBound) {
  int64_t oldBound = 0;
  for (int64_t i = minBound; i <= maxBound; i += minBound) {
    if (tryAdvanced(k, i, oldBound)) {return true;}
    oldBound = i;
  }
  return false;
}
