// Includes
#include "basic.h"
#include "preliminary.h"
#include "advanced.h"
#include <omp.h>
#include <stdlib.h> // for atoll
// these are included in basic.h but I thought it best to include them here for completeness
#include <stdio.h>
#include <inttypes.h>

FILE* resultsDotTxt;

void createResultsTxt() {
  FILE* f = fopen("results.txt", "w"); //creates blank results.txt file.
  fclose(f);
}

void mainloop(uint64_t min, uint64_t max, uint64_t threads, int64_t minDivbound, int64_t maxDivbound) {
  //uint64_t toughs[9] = {64507, 28397, 80502, 68063, 44293, 91163, 72682, 99377, 72717};
  #pragma omp parallel for num_threads(threads)
  for (uint64_t i = min; i <= max; i++) {
  if (tryBasic(i)) {continue;}
    for (int64_t j = minDivbound; j <= maxDivbound; j *= 2) {
      if (tryAdvanced(i, j)) {goto mainloop_continue;}
    }
    if (tryPreliminary(i)) {continue;}
    (void)fprintf(resultsDotTxt, "Fail: %"PRIu64"\n", i);
mainloop_continue:
  }
}

int main(int argc, char** argv) {
  if (argc != 6) {return -1;}
  if (initfactor64("factor64/factor.bin") < 0) {
		(void)fprintf(stderr, "Cannot read factor data\n");
		return -1;
	}
  const uint64_t MIN = atoll(argv[1]);
  const uint64_t MAX = atoll(argv[2]);
  const uint16_t THREADS = atoi(argv[3]);
  const int64_t MIN_DIVBOUND = atoll(argv[4]);
  const int64_t MAX_DIVBOUND = atoll(argv[5]);
  (void)createResultsTxt();
  resultsDotTxt = fopen("results.txt", "a");
  (void)mainloop(MIN, MAX, THREADS, MIN_DIVBOUND, MAX_DIVBOUND);
  fclose(resultsDotTxt);
  return 0;
}
