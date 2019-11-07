#include<stdio.h>
#include<stdlib.h>
#include "util.h"
#define INT_MAX 100000000
#define STRIDE 32

CYCLES find_min(CYCLES a,CYCLES b){
	if(a<b)
		return a;
	return b;
}

int main(){

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
			printf("The L3/Memory Boundary is at %d\n",i);
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

	printf("The L1/L2 Memory Boundary is %d\n",sum/1000);
	free(p);
	free(time);

	return 0;
}