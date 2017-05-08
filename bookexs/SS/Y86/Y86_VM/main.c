#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "y86_system.h"

static unsigned char sample_program[] = {
	0x30, 0xF0, 0xFF, 0xFF, 0x00, 0x00,	/* irmovl $0xFFFF0000, %eax */
	0x30, 0xF1, 0x00, 0x00, 0xFF, 0xFF,	/* irmovl $0x0000FFFF, %ecx */
	0x10,					/* nop */
	0x10,					/* nop */
	0x10,					/* nop */
	0x30, 0xF1, 0xFF, 0xFF, 0x00, 0x00,	/* irmovl $0xFFFF0000, %eax */
	0x30, 0xF0, 0x00, 0x00, 0xFF, 0xFF,	/* irmovl $0x0000FFFF, %ecx */
	0x70, 0x00, 0x00, 0x00, 0x00,		/* jmp 0x00000000 */
	0x00,					/* halt */
};

struct system_run_arg arg = {
	.program = sample_program,
	.size = sizeof(sample_program),
};

void *run_system_thread(void *arg);

int main(int argc, char *argv[])
{
	int res;
	pthread_t cpu_thread;
	void *thread_result;

	pthread_create(&cpu_thread, NULL, run_system_thread, (void *) &arg);

	res = pthread_join(cpu_thread, &thread_result);

	return 0;
}
