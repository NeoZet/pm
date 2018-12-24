#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

void disable_interrupts();
void enable_interrupts();
int enable_rt_scheduler(int priority);
int enable_default_scheduler();
