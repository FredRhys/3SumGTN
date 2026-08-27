// Includes
#include "basic.h"
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
    {
        #pragma omp parallel for num_threads(THREADS)
        for (uint64_t i = 0; i <= RANGE; i++) {
            if (tryBasic(i)) {continue;}
            (void)fprintf(resultsDotTxt, "Fail: %"PRIu64"\n", i);
        }
    }
    fclose(f);
    return 0;
}