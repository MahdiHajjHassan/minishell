#include "minishell.h"

void *get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
	{
		perror("getcwd is dead");
		return NULL;
	}
	return buf;
}

int forkk(void)
{
	int pid;

	pid = fork();
	if (pid == -1)
		wtf();
	return pid;
}

void wtf(void)
{
	printf("wtf\n");
	exit(1);
}
