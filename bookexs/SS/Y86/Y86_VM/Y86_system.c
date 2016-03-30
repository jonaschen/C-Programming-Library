#include "Y86_common.h"

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
	

static struct y86_exec_env y86_system;
static struct y86_exec_env *main_system = &y86_system;
static unsigned char main_memory[MAIN_MEMORY_SIZE];	/* 8K memory */

static void load_program(void)
{
	memcpy(main_system->memory, sample_program, sizeof(sample_program));
}

static void init_system(void)
{
	memset(main_system, 0, sizeof(struct y86_exec_env));

	main_system->memory = main_memory;

	load_program();
}

static void sync_env(struct y86_exec_env *main, struct y86_exec_env *next)
{
	memcpy(next, main, sizeof(struct y86_exec_env));
}

static void dump_y86_registers(struct y86_exec_env *env)
{
	printf("\n[%s]\n", __func__);
	printf("\tPC: 0x%08X\n", env->program_count);
	printf("\teax: 0x%08X\n", env->grf.eax);
	printf("\tecx: 0x%08X\n", env->grf.ecx);
	printf("\tedx: 0x%08X\n", env->grf.edx);
	printf("\tebx: 0x%08X\n", env->grf.ebx);
	printf("\tesi: 0x%08X\n", env->grf.esi);
	printf("\tedi: 0x%08X\n", env->grf.edi);
	printf("\tesp: 0x%08X\n", env->grf.esp);
	printf("\tebp: 0x%08X\n", env->grf.ebp);
}

void *run_system_thread(void *arg)
{
	struct y86_exec_env next_state, *next;
	struct y86_instruction imem;
	int rc = 0;

	next = &next_state;

	init_system();

	while (1) {
		dump_y86_registers(main_system);

		sync_env(main_system, next);

		rc = y86_fetch(main_system, &imem, &next->program_count);
		if (rc)
			break;

		rc = y86_exec(&imem, next);
		if (rc)
			break;

		//y86_memory();

		y86_write_back(&imem, main_system, next);

		usleep(SYSTEM_TICK);
	}

	pthread_exit(NULL);
}
