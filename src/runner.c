/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command(const char *cmd, char **env_copy)
{
	char	*path;
	char	*result;

	path = get_env_value("PATH", 4, env_copy);
	if (! path || ft_strlen(path) == 0)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result != (char *)1)
	{
		free(path);
		return (result);
	}
	result = search_in_paths(path, cmd);
	free(path);
	return (result);
}

void	run_exec_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_execcmd	*ex;

	ex = (struct s_execcmd *)cmd;
	if (ex->av[0] == 0)
		clean_exit(0);
	if (is_builtin(ex->av[0]))
	{
		handle_exec_builtin(ex, cmd, &env_copy);
		return ;
	}
	execute_external_cmd(ex, env_copy);
}

static void	restore_stdin(int saved_stdin)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

static void	restore_stdout(int saved_stdout)
{
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

static void	handle_redir_error(struct s_redircmd *rdir, int saved_stdin,
		int saved_stdout)
{
	set_exit_status(1);
	if (rdir->fd == STDIN_FILENO)
		restore_stdin(saved_stdin);
	else if (rdir->fd == STDOUT_FILENO)
		restore_stdout(saved_stdout);
}

static int	setup_redirection(struct s_redircmd *rdir, int *saved_stdin,
		int *saved_stdout)
{
	if (rdir->fd == STDIN_FILENO)
		*saved_stdin = dup(STDIN_FILENO);
	else if (rdir->fd == STDOUT_FILENO)
		*saved_stdout = dup(STDOUT_FILENO);
	close(rdir->fd);
	return (0);
}

static int	open_redirection_file(struct s_redircmd *rdir, int saved_stdin,
		int saved_stdout)
{
	if ((rdir->mode & O_CREAT) && (rdir->mode & (O_WRONLY | O_RDWR)))
	{
		if (open_redir_file_create(rdir) != 0)
		{
			handle_redir_error(rdir, saved_stdin, saved_stdout);
			return (-1);
		}
	}
	else
	{
		if (open_redir_file_regular(rdir) != 0)
		{
			handle_redir_error(rdir, saved_stdin, saved_stdout);
			return (-1);
		}
	}
	return (0);
}

static void	restore_redirection(struct s_redircmd *rdir, int saved_stdin,
		int saved_stdout)
{
	if (rdir->fd == STDIN_FILENO)
		restore_stdin(saved_stdin);
	else if (rdir->fd == STDOUT_FILENO)
		restore_stdout(saved_stdout);
}

void	run_redir_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_redircmd	*rdir;
	int					saved_stdin;
	int					saved_stdout;

	rdir = (struct s_redircmd *)cmd;
	setup_redirection(rdir, &saved_stdin, &saved_stdout);
	if (open_redirection_file(rdir, saved_stdin, saved_stdout) != 0)
		return ;
	runcmd(rdir->cmd, env_copy);
	restore_redirection(rdir, saved_stdin, saved_stdout);
}

void	run_list_cmd(struct s_cmd *cmd)
{
	(void)cmd;
}

void	runcmd(struct s_cmd *cmd, char **env_copy)
{
	if (cmd == 0)
		clean_exit(0);
	if (cmd->type == EXEC)
		run_exec_cmd(cmd, env_copy);
	else if (cmd->type == REDIR)
		run_redir_cmd(cmd, env_copy);
	else if (cmd->type == HEREDOC)
		run_heredoc_cmd(cmd, env_copy);
	else if (cmd->type == PIPE)
		run_pipe_cmd(cmd, env_copy);
	else if (cmd->type == BACK)
		run_back_cmd(cmd, env_copy);
	else
	{
		ft_fprintf_stderr("unknown command type: %d\n", cmd->type);
		clean_exit(1);
	}
}
