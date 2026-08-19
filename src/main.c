#include "basic.h"
// these are included in basic.h but I thought it best to include them here for completeness
#include <stdio.h>
#include <inttypes.h>
#define RANGE 1000000

int main(void) {
    if (initfactor64("factor64/factor.bin") < 0) {
		fprintf(stderr, "Cannot read factor data\n");
		return -1;
	}
    uint64_t count = 0;
    for (uint64_t i = 0; i <= RANGE; i++) {
        if (tryBasic(i)) {continue;}
        count++;
    }
    fprintf(stdout, "Failed to represent %"PRIu64" integers in the specified range.\n", count);
    return 0;
}