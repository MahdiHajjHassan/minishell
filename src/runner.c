#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

char	*find_command(const char *cmd)
{
	char	*path;
	char	*curr;
	char	*next;
	char	full_path[1024];
	size_t	len;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	curr = path;
	while (curr && *curr)
	{
		next = strchr(curr, ':');
		len = next ? (size_t)(next - curr) : strlen(curr);
		if (len + strlen(cmd) + 2 > sizeof(full_path))
		{
			fprintf(stderr, "Path too long: %.*s/%s\n", (int)len, curr, cmd);
			return (NULL);
		}
		strncpy(full_path, curr, len);
		full_path[len] = '/';
		strcpy(full_path + len + 1, cmd);
		if (access(full_path, X_OK) == 0)
			return (strdup(full_path));
		if (!next)
			break;
		curr = next + 1;
	}
	return (NULL);
}

static void	reset_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	runcmd(struct s_cmd *cmd)
{
	int					p[2];
	struct s_backcmd	*back;
	struct s_execcmd	*ex;
	struct s_listcmd	*list;
	struct s_pipecmd	*pipecmd;
	struct s_redircmd	*rdir;
	char				*cmd_path;
	extern char			**environ;

	if (cmd == 0)
		exit(0);
	switch (cmd->type)
	{
		case EXEC:
			ex = (struct s_execcmd *)cmd;
			if (ex->av[0] == 0)
				exit(0);
			for (int i = 0; ex->av[i]; i++)
			{
				char	*original;

				original = ex->av[i];
				ex->av[i] = expand_variables(original, strlen(original));
				free(original);
			}
			if (is_builtin(ex->av[0]))
			{
				int	status;

				status = handle_builtin(ex->av);
				set_exit_status(status);
				if (cmd->type != LIST)
					exit(status);
				return ;
			}
			cmd_path = find_command(ex->av[0]);
			if (cmd_path)
			{
				reset_signals();
				execve(cmd_path, ex->av, environ);
				perror("execve failed");
				free(cmd_path);
				exit(1);
			}
			else
			{
				fprintf(stderr, "command not found: %s\n", ex->av[0]);
				exit(127);
			}
		case REDIR:
			rdir = (struct s_redircmd *)cmd;
			close(rdir->fd);
			if ((rdir->mode & O_CREAT) && (rdir->mode & (O_WRONLY | O_RDWR)))
			{
				int	fd;

				fd = open(rdir->file, rdir->mode, 0644);
				if (fd < 0)
				{
					fprintf(stderr, "open failed: %s: %s\n", rdir->file, strerror(errno));
					exit(1);
				}
				if (fd != rdir->fd)
				{
					dup2(fd, rdir->fd);
					close(fd);
				}
			}
			else
			{
				if (open(rdir->file, rdir->mode) < 0)
				{
					fprintf(stderr, "open failed: %s: %s\n", rdir->file, strerror(errno));
					exit(1);
				}
			}
			runcmd(rdir->cmd);
			break;
		case LIST:
			list = (struct s_listcmd *)cmd;
			if (list->left->type == EXEC)
			{
				ex = (struct s_execcmd *)list->left;
				if (ex->av[0] && is_builtin(ex->av[0]))
				{
					for (int i = 0; ex->av[i]; i++)
					{
						char	*original;

						original = ex->av[i];
						ex->av[i] = expand_variables(original, strlen(original));
						free(original);
					}
					int	status;

					status = handle_builtin(ex->av);
					set_exit_status(status);
				}
				else
				{
					int	status;

					if (forkk() == 0)
						runcmd(list->left);
					wait(&status);
					if (WIFSIGNALED(status))
						set_exit_status(128 + WTERMSIG(status));
					else
						set_exit_status(WEXITSTATUS(status));
				}
			}
			else
			{
				int	status;

				if (forkk() == 0)
					runcmd(list->left);
				wait(&status);
				if (WIFSIGNALED(status))
					set_exit_status(128 + WTERMSIG(status));
				else
					set_exit_status(WEXITSTATUS(status));
			}
			if (list->right)
			{
				if (list->right->type == EXEC)
				{
					ex = (struct s_execcmd *)list->right;
					if (ex->av[0] && is_builtin(ex->av[0]))
					{
						for (int i = 0; ex->av[i]; i++)
						{
							char	*original;

							original = ex->av[i];
							ex->av[i] = expand_variables(original, strlen(original));
							free(original);
						}
						int	status;

						status = handle_builtin(ex->av);
						set_exit_status(status);
						return ;
					}
				}
				runcmd(list->right);
			}
			return ;
		case PIPE:
			pipecmd = (struct s_pipecmd *)cmd;
			if (pipe(p) < 0)
			{
				perror("pipe failed");
				wtf();
			}
			if (forkk() == 0)
			{
				close(1);
				dup(p[1]);
				close(p[0]);
				close(p[1]);
				runcmd(pipecmd->left);
			}
			if (forkk() == 0)
			{
				close(0);
				dup(p[0]);
				close(p[0]);
				close(p[1]);
				runcmd(pipecmd->right);
			}
			close(p[0]);
			close(p[1]);
			wait(0);
			wait(0);
			break;
		case BACK:
			back = (struct s_backcmd *)cmd;
			if (forkk() == 0)
			{
				if (forkk() == 0)
				{
					runcmd(back->cmd);
				}
				exit(0);
			}
			wait(0);
			break;
		default:
			fprintf(stderr, "unknown command type: %d\n", cmd->type);
			exit(1);
	}
	if (cmd->type != LIST)
		exit(0);
}