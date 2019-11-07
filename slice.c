#include "util.h"

// 128 MB of memory allocated
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}
#define INT_MAX 100000000
#define STRIDE 32

int offsets[] = {4,5,10,11,22,23,24,25,2,3,12,13,16,17,30,31,6,7,8,9,20,21,26,27,0,1,14,15,18,19,28,29};

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

void access_slices(char** arr,int time_or_not,CYCLES** time){
    for(int j=0;j<2;j++){
        char* mypage = arr[j];
        for(int i=0;i<32;i++){
            char* addr = mypage+((2<<16)*offsets[i]);
            if(time_or_not){
                CYCLES* temp = &(time[i/8][8*j+(i%8)]);
                CYCLES start = rdtscp();
                volatile char w = *addr;
                *temp = rdtscp()-start;
            }else{
                volatile char w = *addr;
            }
        }    
    }
}

CYCLES** myfunc(char** arr,int cache_set_no,int random){

    for(int i=0;i<3;i++){
        arr[i] = (char*)malloc(sizeof(char)*(2<<22));
        unsigned long temp = (unsigned long)arr[i];
        arr[i] = (char*)((((temp>>21)+1)<<21) + ((2<<6)*cache_set_no));
    }

    for(int j=0;j<3;j++){
        for(int i=0;i<31;i++){
                volatile char w = arr[j][(2<<16)*i];    
        }    
    }
    for(int i=0;i<31;i++){
        for(int j=0;j<3;j++)
            flush(&arr[j][(2<<16)*i]);
    }

    CYCLES** time = (CYCLES**)malloc(4*sizeof(CYCLES*));
    for(int i=0;i<4;i++)
        time[i] = (CYCLES*)malloc(16*sizeof(CYCLES));

    access_slices(arr,0,NULL);
    //printf("Creating Conflict - %d\n",random);
    for(int i=0;i<4;i++){
        int my_random = random+i*8;
        char* conflict_address = arr[2]+((offsets[random])*(2<<16));    
        volatile char w = *conflict_address;
    }
    
    access_slices(arr,1,time);
    return time;
}

int main (int argv, char** argc) {
    
    int l3_threshold,l2_threshold;
    int cache_set_no = atoi(argc[1]);
    //int pages = 2;
    char** arr = (char**)malloc(3*sizeof(char*));
    int random = rand()%8;
    CYCLES** time = myfunc(arr,cache_set_no,random);

    find_thresholds(&l3_threshold,&l2_threshold);

    // for(int i=0;i<naccesses;i++){
    //     if(time[i]<=l2_threshold)
    //         l2++;
    //     else if(time[i]<=l3_threshold)
    //         l3++;
    //     else{
    //         cache_evicted = order[i];
    //     }
    //     //printf("%d\n",time[i]);
    // }
    //printf("%d %d\n",l2_threshold,l3_threshold);

    // printf("Misses=%d\nL2 hits=%d\nL3 hits=%d\n",naccesses-l2-l3,l2,l3);

    for(int j=0;j<4;j++){
        int l3=0;
        int l2=0;
        int cache_evicted = -1;
        for(int i=0;i<16;i++){
            CYCLES local_time = time[j][i];
            if(local_time<l2_threshold)
                l2++;
            else if(local_time<l3_threshold)
                l3++;
            else
                cache_evicted = i;
        }    
        printf("%d %d\n",16-l3-l2,cache_evicted);
    }
    
    //printf("L2 hits are %d\n",l2 );
    return 0;
}
