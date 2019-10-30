#include<stdio.h>
#define DIFF(A,B) A-B

int main(){
	long int *p ;
	{
		long int c = 10;
		p = &c;
	}
	long int d = 1000;
	printf("%p %p %d",p,&d,*p);
	return 0;
}