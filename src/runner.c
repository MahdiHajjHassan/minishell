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

void	run_redir_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_redircmd	*rdir;
	int					saved_stdin;
	int					saved_stdout;
	int					fd;

	rdir = (struct s_redircmd *)cmd;
	saved_stdin = -1;
	saved_stdout = -1;
	
	// For output redirections, create/truncate the file but don't redirect yet
	// if the inner command is also an output redirection
	if (rdir->fd == STDOUT_FILENO && rdir->cmd && rdir->cmd->type == REDIR)
	{
		struct s_redircmd *inner_rdir = (struct s_redircmd *)rdir->cmd;
		if (inner_rdir->fd == STDOUT_FILENO)
		{
			// Just create/truncate the file without redirecting
			fd = open(rdir->file, rdir->mode, 0644);
			if (fd < 0)
			{
				print_open_failed(rdir->file, strerror(errno));
				set_exit_status(1);
				return;
			}
			close(fd);
			// Continue with the inner command
			runcmd(rdir->cmd, env_copy);
			return;
		}
	}
	
	// Normal redirection handling
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
