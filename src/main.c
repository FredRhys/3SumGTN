// Includes
#include "basic.h"
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

void mainloop(uint64_t range, uint64_t threads) {
    #pragma omp parallel for num_threads(threads)
    for (uint64_t i = 0; i <= range; i++) {
        if (tryBasic(i)) {continue;}
        if (tryAdvanced(i)) {continue;}
        (void)fprintf(resultsDotTxt, "Fail: %"PRIu64"\n", i);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {return -1;}
    if (initfactor64("factor64/factor.bin") < 0) {
		(void)fprintf(stderr, "Cannot read factor data\n");
		return -1;
	}
    const uint64_t RANGE = atoll(argv[1]);
    const uint16_t THREADS = atoi(argv[2]);
    (void)createResultsTxt();
    resultsDotTxt = fopen("results.txt", "a");
    (void)mainloop(RANGE, THREADS);
    fclose(resultsDotTxt);
    return 0;
}