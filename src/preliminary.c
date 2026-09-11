#include "preliminary.h"

static bool isSolution(uint64_t x,  uint64_t y, uint64_t radicand, uint64_t k) {
    if ((uint64_t)x * x * x - x != radicand) {return false;}
    fprintfResults(x, y, y, k);
    return true;
}

bool tryPreliminary(uint64_t k) {
    const uint64_t _6k = 6 * k;
    int64_t x, radicand;
    for (int64_t y = -2 * sqrtl(k); y > -UINT21_MAX; y--) {
        radicand = _6k + 2 * (y - (int64_t)y * y * y);
        x = cbrtl(radicand);
        if (isSolution(x, y, radicand, k)) {return true;}
        if (isSolution(x+1, y, radicand, k)) {return true;}        
    }
    return false;
}