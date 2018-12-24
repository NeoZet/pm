#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/io.h>

#include "fs_ctrl.h"
#include "group_ctrl.h"

void disable_interrupts()
{
	if(iopl(3) < 0) {
		perror("iopl");
		return;
	}
	asm volatile("cli");
}

void enable_interrupts()
{
	asm volatile("sti");
}

int enable_rt_scheduler(int priority)
{
	int ret = 0;
	struct sched_param param = {
		.sched_priority = priority
	};
	if ((ret = sched_setscheduler(getpid(), SCHED_FIFO, &param)) != 0) {
		perror("set_scheduler");
	}
	return ret;	   
}

int enable_default_scheduler()
{
	int ret = 0;
	struct sched_param param = {
		.sched_priority = 0
	};
	if ((ret = sched_setscheduler(getpid(), SCHED_OTHER, &param)) != 0) {
		perror("set_scheduler");
	}
	return ret;
}
