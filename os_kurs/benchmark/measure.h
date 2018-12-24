#pragma once

#include <stdint.h>
#include <limits.h>

struct measurement {
	uint64_t start;
	uint64_t end;
	uint64_t last_result;
	uint64_t max;
	uint64_t min;
	uint64_t ctr;
	uint64_t avg;
	uint64_t diff;
	uint64_t jitter;
};

static __attribute__((always_inline)) inline uint64_t __meas_start()
{
	uint32_t cycles_high, cycles_low;
	asm volatile ("CPUID\n\t"
		      "RDTSC\n\t"
		      "mov %%edx, %0\n\t"
		      "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
		      "%rax", "%rbx", "%rcx", "%rdx");
	return (((uint64_t)cycles_high << 32) | cycles_low);
}


static __attribute__((always_inline)) inline uint64_t __meas_end()
{
	uint32_t cycles_high, cycles_low;
	asm volatile("RDTSCP\n\t"
		     "mov %%edx, %0\n\t"
		     "mov %%eax, %1\n\t"
		     "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
		     "%rax", "%rbx", "%rcx", "%rdx");
	return (((uint64_t)cycles_high << 32) | cycles_low);
}

void meas_init(struct measurement *meas)
{
	meas->start = 0;
	meas->end = 0;
	meas->last_result = 0;
	meas->max = 0;
	meas->min = ULLONG_MAX;
	meas->ctr = 0;
	meas->avg = 0;
	meas->diff = 0;
	meas->jitter = 0;
}

void meas_start(struct measurement *meas)
{
	if (meas == NULL) {
		return;
	}
	meas->start = __meas_start();
}

void meas_end(struct measurement *meas)
{
	uint64_t res = 0;
	if (meas == NULL) {
		return;
	}	
	meas->end = __meas_end();
	if ((res = meas->end - meas->start) > meas->max) {
		meas->max = res;
	}
	if (res < meas->min) {
		meas->min = res;
	}
	meas->ctr++;	
	meas->diff = res;
}

int meas_result(struct measurement *meas)
{
	if (meas == NULL) {
		return -1;
	}
	if (meas->end < 0 || meas->start < 0) {
		return -2;
	}
	meas->last_result = meas->end - meas->start;
	meas->avg = (meas->max - meas->min) / 2; // TODO: create better finding average
	meas->jitter = meas->max - meas->min;
	return 0;
}

void meas_print_results(struct measurement *meas)
{
	printf("[MEASUREMENT RESULT {CLK}]\n"
	       "items   : %lu\n"
	       "avg     : %lu\n"
	       "max     : %lu\n"
	       "min     : %lu\n"
	       "jitter  : %lu\n",
	       meas->ctr,
	       meas->avg,
	       meas->max,
	       meas->min,
	       meas->jitter);
}
