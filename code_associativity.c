//calculating the associativity of the cache
//using the command  getconf -a | grep CACHE we found the no. of sets in the cache to be 64
//each block contains 16 elements (int elements)
//offset is 64*16 = 1024
//hence when we call addresses for the int array at 0, 1024, 2048.... they all will be placed in the same set

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <time.h>
#include <emmintrin.h>

int t;

uint64_t timediff(int a[],int i)
{
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	t=a[i];
	
	asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	 "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	uint64_t start,end;
	start = ( ((uint64_t)cycles_high << 32) | cycles_low );
	end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
	return end-start;
}

int main()
{
	int a[20080]={0};

	t=0;
	for (int i=0;i<20000;i++)
	{
		_mm_clflush(&a[i]);
	}
	
	for (int i=0; i<=13*1024 ; i+=1024)
	{
		
		t=a[i];
		//accessing a[i] element
		printf("accessing %dth block:\n",(i/1024));
		
		for (int j=0; j<=i;j+=1024)
		{printf("latency for accessing %dth block is	%ld\n",(j/1024),timediff(a,j));}
		//inner loop we are accessing the previously brought-in blocks and printing the no. of cycles taken for accessing
	}
	

}