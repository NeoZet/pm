#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <stdarg.h>
#include <linux/random.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "measure.h"
#include "group_ctrl.h"
#include "fs_ctrl.h"

#define BUFFER_BLOCK_SIZE (1024/8*1024)

struct measurement meas;

void workload(void* start_addr,
	      size_t size,
	      uint iterations,
	      size_t* offsets,
	      size_t offsets_size,
	      struct measurement *meas);

struct timespec sleep_timeout = (struct timespec) {.tv_sec=0, .tv_nsec=1000000};

long iterations = 100;
int  cpuid = 3;
size_t buffer_size = BUFFER_BLOCK_SIZE;
char* buffer = NULL;
long random_probes = 1024;
int sleep_ns = 1;

int main(int argc, char *argv[])
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpuid, &cpuset);
	sched_setaffinity(0, sizeof(cpuset), &cpuset);

	meas_init(&meas);
	
	mlockall(MCL_CURRENT | MCL_FUTURE);

	enable_rt_scheduler(98);

	buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	void *mem = buffer;
	
	size_t* offsets_buffer = NULL;
	size_t offsets_buffer_size = 1024;
	offsets_buffer = malloc(sizeof(*offsets_buffer) * offsets_buffer_size);

	int i = 0;
	for (i = 0; i < random_probes; i++) {
		offsets_buffer[i] = (size_t)((double)random() / (double)RAND_MAX * (double) buffer_size);
	}

	workload(mem, buffer_size, iterations, offsets_buffer, offsets_buffer_size, &meas);
	meas_result(&meas);
	meas_print_results(&meas);
	return 0;
}

void workload(void* start_addr,
	      size_t size,
	      uint iterations,
	      size_t* offsets,
	      size_t offsets_size,
	      struct measurement *meas)
{
	unsigned long long i;
	while(iterations--) {
		if (sleep_ns) {
			nanosleep(&sleep_timeout, NULL);
		}
		disable_interrupts();
		meas_start(meas);
		for (i = 0; i < offsets_size; ++i) {
			size_t offset = offsets[i];			
			asm volatile("mov (%0,%1,1), %%eax"
				     :
				     : "r" (start_addr), "r" (offset)
				     : "%eax", "memory");
		}
		meas_end(meas);
		enable_interrupts();
	}	
}
