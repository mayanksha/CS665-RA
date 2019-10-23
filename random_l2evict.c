#include "util.h"

// 128 MB of memory allocated
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}

CYCLES* myfunc(int pages,char** arr){

    for(int i=0;i<pages;i++){
        arr[i] = (char*)malloc(sizeof(char)*(2<<22));
        unsigned long temp = (unsigned long)arr[i];
        arr[i] = (char*)((temp>>21)<<21);
    }

    for(int i=0;i<31;i++){
        for(int j=0;j<pages;j++)
            flush(&arr[j][(2<<16)*i]);
    }

    int occur[31];
    CYCLES *time = (CYCLES*)malloc(pages*31*sizeof(CYCLES));

    for(int j=0;j<pages;j++){
        for(int i=0;i<31;i++)
            occur[i]=0;
        for(int i=0;i<31;i++){
            int index = rand()%32;
            while(occur[index]!=0){
                index = rand()%32;
            }
            occur[index]=1;
            volatile char w = arr[j][(2<<16)*index];
        }    
    }

    for(int j=0;j<pages;j++){
        for(int i=0;i<31;i++)
            occur[i]=0;
        for(int i=0;i<31;i++){
            int index = rand()%32;
            while(occur[index]!=0){
                index = rand()%32;
            }
            occur[index]=1;
            CYCLES start;
            start = rdtscp ();
            volatile char w = arr[j][(2<<16)*index];
            time[index+31*j] = rdtscp()-start;
        }
    }
    
    return time;
}
int main (int argv, char** argc) {
    
    int pages = 4;
    char** arr = (char**)malloc(pages*sizeof(char*));
    CYCLES* time = myfunc(pages,arr);
    // volatile int *xx = malloc(sizeof(int));
    // int yyy,yy;
    // *xx=1;
    // CYCLES start;
    // yyy = *xx;
    // start = rdtscp();
    // yy = *xx;
    // CYCLES tt= rdtscp()-start;
    // printf("time hit %d\n",tt);        //code proves that L1/L2 hit is 50 cycles max.
    
    int count=0;
    for(int i=0;i<pages*31;i++){
        if(time[i]<=240 && time[i]>=60){
            count++;
        }
        printf("%d\n",time[i]);
    }
    printf("%d",count);
    return 0;
}
