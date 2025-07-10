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

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char *line;
	struct s_cmd *cmd;
	struct s_execcmd *ecmd;
	int status;

	while (1)
	{
		line = readline_helper();
		if (!line)
			break;
		cmd = tokenize(line);
		if (cmd->type == EXEC) {
			ecmd = (struct s_execcmd *)cmd;
			if (ecmd->av[0] && is_builtin(ecmd->av[0])) {
				status = handle_builtin(ecmd->av);
				set_exit_status(status);
				free(line);
				continue;
			}
		}
		if (forkk() == 0) {
			runcmd(cmd);
			exit(0);
		}
		wait(&status);
		set_exit_status(status);
		free(line);
	}
	return (g_last_exit_status);
}
