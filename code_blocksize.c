//calculating the block size

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <time.h>
#include <emmintrin.h>

int t;

//this function returns the no. of cycles taken for accessing a[i]
uint64_t timediff(int a[],int i)
{
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	t=a[i];//trying to access a[i]
	
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
	int a[240]={0};//initialising the array

	t=0;
	for (int i=0;i<240;i++)
	{
		_mm_clflush(&a[i]);
		//flushing all values of the array from the cache
	}
	
	for (int i=0;i<240;i++)
	{
	
		printf("%ld,%d\n",timediff(a,i),i);
		//printing the cycles taken for accessing a[i]
	}

}