#include "util.h"

// 128 MB of memory allocated
#define STRIDE_LENGTH 512LL

const long long HUGE_PAGE_SIZE =  2 << 20;
const long long NORMAL_PAGE_SIZE =  4 << 10;

struct s_access {
    CYCLES read_cycles;
    long long offset;
};

long long compare_function(const void *a,const void *b) {
    struct s_access *x = (struct s_access *) a;
    struct s_access *y = (struct s_access *) b;
    return (*x).read_cycles - (*y).read_cycles;
}

int main (int argv, char** argc) {
    long long N_ACCESSES;
    long long MEM_TO_ALLOC;
    if (argv == 2) {
        // Argument means memory to allocate in MB
        MEM_TO_ALLOC = (atoi(argc[1]) << 20);
        N_ACCESSES = MEM_TO_ALLOC / STRIDE_LENGTH;
    } else {
        MEM_TO_ALLOC = 16 << 20;
        N_ACCESSES = MEM_TO_ALLOC / STRIDE_LENGTH;
    }

    char *data = malloc (sizeof (char) * MEM_TO_ALLOC);
    struct s_access accesses[N_ACCESSES];

    memset (data, '\0', N_ACCESSES * sizeof (struct s_access));

    for (int i = 0; i < N_ACCESSES; i++)
        accesses[i].offset = rand() % (N_ACCESSES);
        /* accesses[i].offset = i; */

    for (int i = 0; i < N_ACCESSES; i++) {
        volatile char x = data[accesses[i].offset * STRIDE_LENGTH];
    }

    for (int i = 0; i < N_ACCESSES; i++) {
        CYCLES start;
        start = rdtscp ();
        volatile char x = data[accesses[i].offset * STRIDE_LENGTH];

        accesses[i].read_cycles = rdtscp () - start;
    }

    qsort (accesses, sizeof(accesses)/sizeof(struct s_access), sizeof(struct s_access), compare_function);

    for (int i = 0; i < N_ACCESSES; i++) {
        /* printf ("%10lld,%lu\n", accesses[i].offset * STRIDE_LENGTH, accesses[i].read_cycles); */
        printf ("%ld\n", accesses[i].read_cycles);
    }
    /* printf ("Addresses accessed = %lld\n", N_ACCESSES); */
}
