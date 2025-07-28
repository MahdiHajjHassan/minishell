/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_path(const char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	return ((char *)1);
}

size_t	get_path_segment_len(char *curr, char **next)
{
	size_t	len;

	*next = strchr(curr, ':');
	if (*next)
		len = (size_t)(*next - curr);
	else
		len = strlen(curr);
	return (len);
}

int	build_full_path(char *full_path, char *curr,
					size_t len, const char *cmd)
{
	if (len + strlen(cmd) + 2 > 1024)
	{
		fprintf(stderr, "Path too long: %.*s/%s\n", (int)len, curr, cmd);
		return (1);
	}
	strncpy(full_path, curr, len);
	full_path[len] = '/';
	strcpy(full_path + len + 1, cmd);
	return (0);
}

char	*search_in_paths(char *path, const char *cmd)
{
	char	*curr;
	char	*next;
	char	full_path[1024];
	size_t	len;

	curr = path;
	while (curr && *curr)
	{
		len = get_path_segment_len(curr, &next);
		if (build_full_path(full_path, curr, len, cmd))
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (strdup(full_path));
		if (!next)
			break ;
		curr = next + 1;
	}
	return (NULL);
}

void	reset_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	expand_exec_args(struct s_execcmd *ex)
{
	int		i;
	char	*original;

	i = 0;
	while (ex->av[i])
	{
		original = ex->av[i];
		ex->av[i] = expand_variables(original, strlen(original));
		free(original);
		i++;
	}
}

void	handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd)
{
	int	status;

	status = handle_builtin(ex->av);
	set_exit_status(status);
	if (cmd->type != LIST)
		exit(status);
}

void	execute_external_cmd(struct s_execcmd *ex)
{
	char			*cmd_path;
	extern char		**environ;

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
}

int	open_redir_file_create(struct s_redircmd *rdir)
{
	int	fd;

	fd = open(rdir->file, rdir->mode, 0644);
	if (fd < 0)
	{
		fprintf(stderr, "open failed: %s: %s\n", rdir->file,
			strerror(errno));
		exit(1);
	}
	if (fd != rdir->fd)
	{
		dup2(fd, rdir->fd);
		close(fd);
	}
	return (0);
}

void	open_redir_file_regular(struct s_redircmd *rdir)
{
	if (open(rdir->file, rdir->mode) < 0)
	{
		fprintf(stderr, "open failed: %s: %s\n", rdir->file,
			strerror(errno));
		exit(1);
	}
}

void	handle_list_builtin(struct s_execcmd *ex)
{
	int	status;

	expand_exec_args(ex);
	status = handle_builtin(ex->av);
	set_exit_status(status);
}

void	handle_list_external(struct s_cmd *cmd)
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

void	process_list_left(struct s_listcmd *list)
{
	struct s_execcmd	*ex;

	if (list->left->type == EXEC)
	{
		ex = (struct s_execcmd *)list->left;
		if (ex->av[0] && is_builtin(ex->av[0]))
		{
			handle_list_builtin(ex);
		}
		else
		{
			handle_list_external(list->left);
		}
	}
	else
	{
		handle_list_external(list->left);
	}
}

void	process_list_right(struct s_listcmd *list)
{
	struct s_execcmd	*ex;

	if (list->right)
	{
		if (list->right->type == EXEC)
		{
			ex = (struct s_execcmd *)list->right;
			if (ex->av[0] && is_builtin(ex->av[0]))
			{
				handle_list_builtin(ex);
				return ;
			}
		}
		runcmd(list->right);
	}
}

void	setup_pipe_left(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->left);
	}
}

void	setup_pipe_right(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->right);
	}
}

void	run_pipe_cmd(struct s_cmd *cmd)
{
	int					p[2];
	struct s_pipecmd	*pipecmd;

	pipecmd = (struct s_pipecmd *)cmd;
	if (pipe(p) < 0)
	{
		perror("pipe failed");
		wtf();
	}
	setup_pipe_left(p, pipecmd);
	setup_pipe_right(p, pipecmd);
	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
}

void	run_back_cmd(struct s_cmd *cmd)
{
	struct s_backcmd	*back;

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
} 