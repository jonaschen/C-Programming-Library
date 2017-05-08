#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "y86_system.h"

#define SYSTEM_TICK_UNIT_MS	(50 * 1000)
#define SYSTEM_TICK_SCALE	20
#define SYSTEM_TICK		(SYSTEM_TICK_UNIT_MS * SYSTEM_TICK_SCALE)


struct y86_cpu_system {
	struct y86_cpu_core *core;
	void *memory;
};

#define MAIN_MEMORY_SIZE	(8 * 1024)
static unsigned char main_memory[MAIN_MEMORY_SIZE];	/* 8K memory */

static void load_program(void *memory, unsigned char *program, size_t size)
{
	memcpy(memory, program, size);
}

#define SYSTEM_RESET_PC		(0U)
static void y86_cpu_system_init(struct y86_cpu_system *system, void *main_memory)
{
	system->memory = main_memory;
	system->core = y86_cpu_core_new(SYSTEM_RESET_PC);
}

struct y86_cpu_system *y86_cpu_system_new(unsigned char *main_memory)
{
	struct y86_cpu_system *system;

	system = (struct y86_cpu_system *) malloc(sizeof(struct y86_cpu_system));

	y86_cpu_system_init(system, main_memory);

	return system;
}

void y86_cpu_system_cleanup(struct y86_cpu_system *system)
{
	free(system);
}

static void dump_y86_registers(struct y86_cpu_system *system)
{
	struct y86_cpu_core *core = system->core;
	struct register_files *gpr = &core->gpr;

	printf("\n[%s]\n", __func__);
	printf("\tPC: 0x%08X\n", core->program_count);
	printf("\teax: 0x%08X\n", gpr->eax);
	printf("\tecx: 0x%08X\n", gpr->ecx);
	printf("\tedx: 0x%08X\n", gpr->edx);
	printf("\tebx: 0x%08X\n", gpr->ebx);
	printf("\tesi: 0x%08X\n", gpr->esi);
	printf("\tedi: 0x%08X\n", gpr->edi);
	printf("\tesp: 0x%08X\n", gpr->esp);
	printf("\tebp: 0x%08X\n", gpr->ebp);
}

void *run_system_thread(void *arg)
{
	struct system_run_arg *prog = (struct system_run_arg *) arg;
	struct y86_cpu_system *system;
	struct y86_instruction imem;
	int rc = 0;

	load_program(main_memory, prog->program, prog->size);

	system = y86_cpu_system_new(main_memory);

	while (1) {
		dump_y86_registers(system);

		rc = system->core->fetch(system->core, &imem, (unsigned char *) system->memory);
		if (rc)
			break;

		rc = system->core->exec(system->core, &imem);
		if (rc)
			break;

		//y86_memory();

		//y86_write_back(&imem, main_system, next);

		usleep(SYSTEM_TICK);
	}

	y86_cpu_system_cleanup(system);

	pthread_exit(NULL);
}
