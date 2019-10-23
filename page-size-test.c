#include "util.h"

// 128 MB of memory allocated
#define STRIDE_LENGTH 512LL

const long long MEM_TO_ALLOC =  8 << 20;
const long long HUGE_PAGE_SIZE =  2 << 20;
const long long NORMAL_PAGE_SIZE =  4 << 10;
const long long N_ACCESSES = MEM_TO_ALLOC / STRIDE_LENGTH;

struct s_access {
    CYCLES read_cycles;
    long long offset;
};

int compare_function(const void *a,const void *b) {
    struct s_access *x = (struct s_access *) a;
    struct s_access *y = (struct s_access *) b;
    return (*x).offset - (*y).offset;
}

int main () {
    char *data = malloc (sizeof (char) * MEM_TO_ALLOC);
    struct s_access accesses[N_ACCESSES];

    CYCLES out_start, cycles_taken;

    memset (data, '0', MEM_TO_ALLOC);

    for (int i = 0; i < N_ACCESSES; i++)
        /* accesses[i].offset = rand() % (N_ACCESSES); */
        accesses[i].offset = i;

    out_start = rdtscp ();
    for (int i = 0; i < N_ACCESSES; i++) {
        CYCLES start;

        start = rdtscp ();
        volatile char x = data[accesses[i].offset * STRIDE_LENGTH];
    }

    /* printf ("Mem to alloc = %lld\n", MEM_TO_ALLOC); */
    cycles_taken = rdtscp () - out_start;

    for (int i = 0; i < N_ACCESSES; i++) {
        CYCLES start;
        start = rdtscp ();
        volatile char x = data[accesses[i].offset * STRIDE_LENGTH];

        accesses[i].read_cycles = rdtscp () - start;
    }

    /* qsort (accesses, sizeof(accesses)/sizeof(struct s_access), sizeof(struct s_access), compare_function); */

    for (int i = 0; i < N_ACCESSES; i++) {
        printf ("[%lld]\t %lu\n", accesses[i].offset * STRIDE_LENGTH, accesses[i].read_cycles);
    }
    printf ("Cycles Taken = %lu\n", cycles_taken);
}
