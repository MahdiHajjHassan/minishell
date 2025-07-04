#include "minishell.h"

char *readline_helper(void)
{
	char	*buf;
	size_t	size = 0;
	char	cwd[1024];

	buf = NULL;
	get_cwd(cwd, sizeof(cwd));
	if (isatty(fileno(stdin)))
		printf("%s> ", cwd);
	if (getline(&buf, &size, stdin) == -1)
	{
		if (feof(stdin))
		{
			printf("[EOF]\n");
			free(buf);
			return NULL;
		}
		else
		{
			printf("Getline is dead\n");
			free(buf);
			return NULL;
		}
	}
	return (buf);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	char *line;
	int	fd;

	while ((fd = open("console", O_RDWR)) >= 0)
	{
		if (fd > 2)
			close(fd);
		break;
	}
	while (1)
	{
		line = readline_helper();
		if (!line)
			break;
		if (forkk() == 0) {
			runcmd(tokenize(line));
		}
		wait(0);
		printf("%s", line);
		free(line);
	}
    return (EXIT_SUCCESS);
}
