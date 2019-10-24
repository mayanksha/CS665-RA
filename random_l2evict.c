#include "util.h"

// 128 MB of memory allocated
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}

void access_the_blocks(int time_or_not, int pages,int naccesses,char** arr,CYCLES* time,int* order){

    int occur[31];
    int index_of_block=0;
    for(int j=0;j<pages;j++){
        int iter = 31;
        for(int i=0;i<31;i++)
            occur[i]=0;
        if(j==pages-1)
            iter = naccesses-((pages-1)*31);

        for(int i=0;i<iter;i++){
            int index = rand()%(iter);
            while(occur[index]!=0){
                index = rand()%(iter);
            }
            occur[index]=1;
            if(time_or_not){
                CYCLES start;
                start = rdtscp ();
                volatile char w = arr[j][(2<<16)*index];
                time[index+31*j] = rdtscp()-start;    
            }else{
                volatile char w = arr[j][(2<<16)*index];    
                order[index+31*j] = index_of_block;
                index_of_block++;
            }
        }    
    }
}

CYCLES* myfunc(int pages,char** arr,int naccesses,int* order){

    for(int i=0;i<pages;i++){
        arr[i] = (char*)malloc(sizeof(char)*(2<<22));
        unsigned long temp = (unsigned long)arr[i];
        arr[i] = (char*)(((temp>>21)+1)<<21);
        //printf("%p %p\n",temp,arr[i]);
    }

    for(int i=0;i<31;i++){
        for(int j=0;j<pages;j++)
            flush(&arr[j][(2<<16)*i]);
    }

    CYCLES *time = (CYCLES*)malloc(naccesses*sizeof(CYCLES));
    access_the_blocks(0,pages,naccesses,arr,NULL,order);
    access_the_blocks(1,pages,naccesses,arr,time,NULL);
    
    return time;
}

int main (int argv, char** argc) {
    
    int naccesses = atoi(argc[1]);
    int pages = (naccesses/31)+1;
    //int pages = 2;
    char** arr = (char**)malloc(pages*sizeof(char*));
    int *order = (CYCLES*)malloc(naccesses*sizeof(int));
    CYCLES* time = myfunc(pages,arr,naccesses,order);

    // volatile int *xx = malloc(sizeof(int));
    // int yyy,yy;
    // *xx=1;
    // CYCLES start;
    // yyy = *xx;
    // start = rdtscp();
    // yy = *xx;
    // CYCLES tt= rdtscp()-start;
    // printf("time hit %d\n",tt);        //code proves that L1/L2 hit is 50 cycles max.
    
    int memory[naccesses];
    for(int i=0;i<naccesses;i++)
        memory[i] = -1;

    int l3=0;
    int l2=0;
    int cache_evicted = -1;
    for(int i=0;i<naccesses;i++){
        if(time[i]<=230 && time[i]>=60){
            l3++;
        }
        else if(time[i]<60)
            l2++;
        else{
            cache_evicted = order[i];
        }
        //printf("%d\n",time[i]);
    }

    printf("%d\n%d\n",naccesses-l2-l3,cache_evicted);
    //printf("L3 hits are %d\n",count);
    //printf("L2 hits are %d\n",l2 );
    return 0;
}
