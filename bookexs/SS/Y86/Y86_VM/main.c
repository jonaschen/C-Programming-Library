#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *run_system_thread(void *arg);

int main(int argc, char *argv[])
{
	int res;
	pthread_t cpu_thread;
	void *thread_result;

	pthread_create(&cpu_thread, NULL, run_system_thread, NULL);

	res = pthread_join(cpu_thread, &thread_result);

	return 0;
}
