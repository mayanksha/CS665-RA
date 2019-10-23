#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#define SAMPLES 220
#define CACHE_LINE 64
#define STRIDE 4096
//  Code to demonstrate Hardware Prefetching!
extern inline __attribute__((always_inline))
inline int time_access_no_flush(const char *adrs){
  volatile unsigned long time;
  asm __volatile__ (
    "  mfence             \n"
    "  lfence             \n"
    "  rdtsc              \n"
    "  lfence             \n"
    "  movl %%eax, %%esi  \n"
    "  movl (%1), %%eax   \n"
    "  lfence             \n"
    "  rdtsc              \n"
    "  subl %%esi, %%eax  \n"
    : "=a" (time)
    : "c" (adrs)
    :  "%esi", "%edx");
  return time;
}

//IGNORE CS665
extern inline __attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}


int main (int argc, char **argv)
{

	int i;
	char *a;
	a=(char*)malloc(1000000*sizeof(char));
 	for(i=0;i<1000000;i++)
 		a[i]='a';
  	unsigned int *times = (unsigned int*) malloc(SAMPLES * sizeof(unsigned int));
  	if (times == NULL) {
    	return 1;
  	}
  	
  	// flush(a);
  	for(i=0;i<1000000;i++)
 		flush(a+i);
  	// First Access 
  	for (i = 0; i < SAMPLES; i++) {
   		times[i] = time_access_no_flush(a+i*128);
  	}

  	for (i = 0; i < SAMPLES; i++) {
    	printf("%d %u\n",i, times[i]);
  	}

  	// Second Access 
  	for (i = 0; i < SAMPLES; i++) {
    	times[i] = time_access_no_flush(a+i*128);
  	}
  	for (i = 0; i < SAMPLES; i++) {
    	printf("%d %u\n",i, times[i]);
  	}

  return 0;
}
