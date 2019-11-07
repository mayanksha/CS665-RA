#include "util.h"

// 128 MB of memory allocated
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}
#define INT_MAX 100000000
#define STRIDE 32

CYCLES find_min(CYCLES a,CYCLES b){
    if(a<b)
        return a;
    return b;
}

void find_thresholds(int* l3_threshold,int* l2_threshold){

    int* p = (int*)malloc(INT_MAX*sizeof(int));
    for(long int i=0;i<INT_MAX;i+=STRIDE){
        volatile int w = p[i];
    }
    CYCLES max = 0;
    CYCLES* time = (CYCLES*)malloc(((INT_MAX/STRIDE)+1)*sizeof(CYCLES));
    for(long int i=0;i<INT_MAX;i+=STRIDE){
        CYCLES start;
        start = rdtscp ();
        volatile int w = p[i];
        time[i/STRIDE] = rdtscp()-start;
        if(max<time[i/STRIDE])
            max = time[i/STRIDE];
    }
    
    max = find_min(2000,max);
    long int arr[max+1];
    printf("%d\n",max);
    for(int i=0;i<=max;i++){
        arr[i]=0;
    }
    for(long int i=0;i<INT_MAX;i+=STRIDE){
        if(time[i/STRIDE]<=2000)
            arr[time[i/STRIDE]]++;
    }
    int count=0;
    int flag=0;
    for(int i=0;i<=max;i++){
        count+=arr[i];
        if(count>74752){
            *l3_threshold = i;
            break;
        }
    }

    CYCLES temp = 0;
    CYCLES sum =0;
    for(int i=0;i<1000;i++){
        volatile int w = time[0];
        CYCLES start = rdtscp();
        volatile int q = time[0];
        temp = rdtscp()-start;
        sum +=temp;
    }

    *l2_threshold = sum/1000;
    free(p);
    free(time);
}

int max(int a,int b){
    if(a>b)
        return a;
    return b;
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


CYCLES* myfunc(int pages,char** arr,int naccesses,int* order,int cache_set_no){

    for(int i=0;i<max(pages,3);i++){
        arr[i] = (char*)malloc(sizeof(char)*(2<<22));
        unsigned long temp = (unsigned long)arr[i];
        arr[i] = (char*)((((temp>>21)+1)<<21) + ((2<<6)*cache_set_no));
        //printf("%p %p\n",temp,arr[i]);
    }

    access_the_blocks(0,3,93,arr,NULL,order);
    for(int i=0;i<31;i++){
        for(int j=0;j<max(pages,3);j++)
            flush(&arr[j][(2<<16)*i]);
    }

    CYCLES *time = (CYCLES*)malloc(naccesses*sizeof(CYCLES));
    access_the_blocks(0,pages,naccesses,arr,NULL,order);
    access_the_blocks(1,pages,naccesses,arr,time,NULL);
    
    return time;
}

int main (int argv, char** argc) {
    
    int l3_threshold,l2_threshold;

    int naccesses = atoi(argc[1]);
    int cache_set_no = atoi(argc[2]);
    int pages = (naccesses/31)+1;
    //int pages = 2;
    char** arr = (char**)malloc(max(pages,3)*sizeof(char*));
    int *order = (int*)malloc(max(naccesses,93)*sizeof(int));
    CYCLES* time = myfunc(pages,arr,naccesses,order,cache_set_no);

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
    //for(int i=0;i<naccesses;i++)
        //printf("%d\n",time[i]);
    find_thresholds(&l3_threshold,&l2_threshold);

    for(int i=0;i<naccesses;i++){
        if(time[i]<=l2_threshold)
            l2++;
        else if(time[i]<=l3_threshold)
            l3++;
        else{
            cache_evicted = order[i];
        }
        //printf("%d\n",time[i]);
    }
    printf("%d %d\n",l2_threshold,l3_threshold);

    printf("Misses=%d\nL2 hits=%d\nL3 hits=%d\n",naccesses-l2-l3,l2,l3);
    //printf("L3 hits are %d\n",count);
    //printf("L2 hits are %d\n",l2 );
    return 0;
}
