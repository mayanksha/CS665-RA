#include "util.h"

// 128 MB of memory allocated
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}

int main (int argv, char** argc) {
    char* x = (char*)malloc(sizeof(char)*(2<<22));

    unsigned long temp = (unsigned long)x;
    char* y = (temp>>21)<<21;
    //printf("%x %x",x,y);
    for(int i=0;i<31;i++){
        flush(&y[(2<<16)*i]);
    }
    for(int i=0;i<31;i++){
        volatile char w = y[(2<<16)*i];
    }
    
    CYCLES time[31];
    for(int i=0;i<31;i++){
        CYCLES start;
        start = rdtscp ();
        volatile char w = y[(2<<16)*i];
        time[i] = rdtscp()-start;
    }
    
    int count=0;
    for(int i=0;i<31;i++){
        if(time[i]<=220 && time[i]>=50){
            count++;
        }
        printf("%d\n",time[i]);
    }
    printf("L3 Hits = %d",count);
    return 0;
}
