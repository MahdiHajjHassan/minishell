#include "minishell.h"

void	*get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
	{
		perror("getcwd is dead");
		return (NULL);
	}
	return (buf);
}

int	forkk(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		wtf();
	return (pid);
}

void	wtf(void)
{
	printf("wtf\n");
	exit(1);
}

struct s_cmd	*nulterm(struct s_cmd *cmd)
{
	int					i;
	struct s_backcmd	*bcmd;
	struct s_execcmd	*ecmd;
	struct s_listcmd	*lcmd;
	struct s_pipecmd	*pcmd;
	struct s_redircmd	*rcmd;

	i = 0;
	if (cmd == 0)
		return (0);
	switch (cmd->type)
	{
		case EXEC:
			ecmd = (struct s_execcmd *)cmd;
			for (i = 0; ecmd->av[i]; i++)
				*ecmd->eav[i] = 0;
			break;
		case REDIR:
			rcmd = (struct s_redircmd *)cmd;
			nulterm(rcmd->cmd);
			*rcmd->efile = 0;
			break;
		case PIPE:
			pcmd = (struct s_pipecmd *)cmd;
			nulterm(pcmd->left);
			nulterm(pcmd->right);
			break;
		case LIST:
			lcmd = (struct s_listcmd *)cmd;
			nulterm(lcmd->left);
			nulterm(lcmd->right);
			break;
		case BACK:
			bcmd = (struct s_backcmd *)cmd;
			nulterm(bcmd->cmd);
			break;
	}
	return (cmd);
}
