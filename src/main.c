#include "basic.h"

int main(void) {
    if (initfactor64("factor64/factor.bin") < 0) {
		fprintf(stderr, "Cannot read factor data\n");
		return -1;
	}
    return 0;
}