#include "util.h"

/*
 * Returns Time Stamp Counter
 */
extern inline __attribute__((always_inline))
CYCLES rdtscp(void) {

  CYCLES cycles;
  asm volatile (
    " rdtscp  \n\t"
    : "=a" (cycles));

	return cycles;
}
