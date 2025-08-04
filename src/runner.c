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
	if (ex->av[0] && ft_strlen(ex->av[0]) == 0)
	{
		set_exit_status(0);
		return ;
	}
	if (is_builtin(ex->av[0]))
	{
		handle_exec_builtin(ex, cmd, &env_copy);
		return ;
	}
	execute_external_cmd(ex, env_copy);
}

static int	validate_redirection_chain(struct s_cmd *cmd)
{
	struct s_redircmd	*rdir;

	if (!cmd || cmd->type != REDIR)
		return (0);
	rdir = (struct s_redircmd *)cmd;
	if (rdir->fd == STDIN_FILENO)
	{
		if (access(rdir->file, F_OK) != 0)
		{
			print_open_failed(rdir->file, strerror(errno));
			return (1);
		}
	}
	return (validate_redirection_chain(rdir->cmd));
}

void	run_redir_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_redircmd	*rdir;
	int					saved_stdin;
	int					saved_stdout;
	int					redir_failed;

	rdir = (struct s_redircmd *)cmd;
	if (validate_redirection_chain(cmd) != 0)
	{
		set_exit_status(1);
		return ;
	}
	setup_redirection(rdir, &saved_stdin, &saved_stdout);
	redir_failed = open_redirection_file(rdir, saved_stdin, saved_stdout);
	if (redir_failed != 0)
	{
		restore_redirection(rdir, saved_stdin, saved_stdout);
		return ;
	}
	runcmd(rdir->cmd, env_copy);
	restore_redirection(rdir, saved_stdin, saved_stdout);
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
