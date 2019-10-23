#include "util.h"

// 128 MB of memory allocated
#define STRIDE_LENGTH 512LL

const long long MEM_TO_ALLOC =  16 << 20;
const long long HUGE_PAGE_SIZE =  2 << 20;
const long long NORMAL_PAGE_SIZE =  4 << 10;
const long long N_ACCESSES = MEM_TO_ALLOC / STRIDE_LENGTH;

int main () {
    char *data = malloc (sizeof (char) * MEM_TO_ALLOC);
    int rand_access[N_ACCESSES];

    CYCLES read_cycles[N_ACCESSES];
    CYCLES out_start, cycles_taken;

    memset (data, '0', MEM_TO_ALLOC);

    for (int i = 0; i < N_ACCESSES; i++)
        rand_access[i] = rand() % (N_ACCESSES);

    out_start = rdtscp ();
    for (int i = 0; i < N_ACCESSES; i++) {
        CYCLES start;

        start = rdtscp ();
        volatile char x = data[rand_access[i] * STRIDE_LENGTH];
    }

    /* printf ("Mem to alloc = %lld\n", MEM_TO_ALLOC); */
    cycles_taken = rdtscp () - out_start;

    for (int i = 0; i < N_ACCESSES; i++) {
        CYCLES start;
        start = rdtscp ();
        volatile char x = data[rand_access[i] * STRIDE_LENGTH];

        read_cycles[i] = rdtscp () - start;
    }

    for (int i = 0; i < N_ACCESSES; i++) {
        printf ("[%lld]\t %lu\n", rand_access[i] * STRIDE_LENGTH, read_cycles[i]);
    }
    printf ("Cycles Taken = %lu\n", cycles_taken);
}
