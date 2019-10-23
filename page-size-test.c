#include "util.h"

// 16 MB of memory allocated
const long long MEM_TO_ALLOC =  128 << 20;
const long long HUGE_PAGE_SIZE =  2 << 20;
const long long NORMAL_PAGE_SIZE =  4 << 10;

int main () {
    char *data = malloc (sizeof (char) * MEM_TO_ALLOC);
    CYCLES read_cycles[MEM_TO_ALLOC / NORMAL_PAGE_SIZE];
    CYCLES out_start, cycles_taken;

    memset (data, '0', MEM_TO_ALLOC);

    out_start = rdtscp ();
    for (int i = 0; i < MEM_TO_ALLOC / NORMAL_PAGE_SIZE; i++) {
        CYCLES start;

        start = rdtscp ();
        volatile char x = data[i];

        /* read_cycles[i] = rdtscp () - start; */
        /* volatile char y = x - (i % '0'); */
    }
    cycles_taken = rdtscp () - out_start;

    /* for (int i = 0; i < MEM_TO_ALLOC / NORMAL_PAGE_SIZE; i++) {
     *     printf ("[%d]\t %lu\n", i, read_cycles[i]);
     * } */
    printf ("Cycles Taken = %lu\n", cycles_taken);
}
