#include "minishell.h"

/*
 * handle_builtin_in_list - Handle builtin command in list context
 *
 * @ex: Exec command structure
 *
 * Returns: 1 if handled, 0 if not a builtin
 */
static int	handle_builtin_in_list(struct s_execcmd *ex)
{
	int	status;

	if (ex->av[0] && is_builtin(ex->av[0]))
	{
		status = handle_builtin(ex->av);
		set_exit_status(status);
		return (1);
	}
	return (0);
}

/*
 * handle_fork_and_wait - Fork and wait for command
 *
 * @cmd: Command to execute
 */
static void	handle_fork_and_wait(struct s_cmd *cmd)
{
	int	status;

	if (forkk() == 0)
		runcmd(cmd);
	wait(&status);
	if (WIFSIGNALED(status))
		set_exit_status(128 + WTERMSIG(status));
	else
		set_exit_status(WEXITSTATUS(status));
}

/*
 * handle_list_cmd - Handle LIST command type
 *
 * @cmd: Command structure to execute
 */
void	handle_list_cmd(struct s_cmd *cmd)
{
	struct s_listcmd	*list;
	struct s_execcmd	*ex;
	int					status;

	list = (struct s_listcmd *)cmd;
	// Handle left command
	if (list->left->type == EXEC)
	{
		ex = (struct s_execcmd *)list->left;
		if (!handle_builtin_in_list(ex))
		{
			handle_fork_and_wait(list->left);
		}
	}
	else
	{
		handle_fork_and_wait(list->left);
	}
	// Handle right command
	if (list->right)
	{
		if (list->right->type == EXEC)
		{
			ex = (struct s_execcmd *)list->right;
			if (ex->av[0] && is_builtin(ex->av[0]))
			{
				status = handle_builtin(ex->av);
				set_exit_status(status);
				return ;
			}
		}
		runcmd(list->right);
	}
}

/*
 * handle_pipe_cmd - Handle PIPE command type
 *
 * @cmd: Command structure to execute
 */
void	handle_pipe_cmd(struct s_cmd *cmd)
{
	struct s_pipecmd *pipecmd = (struct s_pipecmd *)cmd;
	int p[2];

	if (pipe(p) < 0)
	{
		perror("pipe failed");
		wtf();
	}

	// Left side of pipe
	if (forkk() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->left);
	}

	// Right side of pipe
	if (forkk() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->right);
	}

	// Parent closes pipe and waits
	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
}