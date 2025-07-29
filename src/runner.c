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

char	*find_command(const char *cmd)
{
	char	*path;
	char	*result;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result != (char *)1)
		return (result);
	return (search_in_paths(path, cmd));
}

void	run_exec_cmd(struct s_cmd *cmd)
{
	struct s_execcmd	*ex;

	ex = (struct s_execcmd *)cmd;
	if (ex->av[0] == 0)
		clean_exit(0);
	expand_exec_args(ex);
	if (is_builtin(ex->av[0]))
	{
		handle_exec_builtin(ex, cmd);
		return ;
	}
	execute_external_cmd(ex);
}

void	run_redir_cmd(struct s_cmd *cmd)
{
	struct s_redircmd	*rdir;

	rdir = (struct s_redircmd *)cmd;
	close(rdir->fd);
	if ((rdir->mode & O_CREAT) && (rdir->mode & (O_WRONLY | O_RDWR)))
	{
		if (open_redir_file_create(rdir) != 0)
		{
			set_exit_status(1);
			clean_exit(1); // Exit with error if redirection fails
		}
	}
	else
	{
		if (open_redir_file_regular(rdir) != 0)
		{
			set_exit_status(1);
			clean_exit(1); // Exit with error if redirection fails
		}
	}
	runcmd(rdir->cmd);
}

/* List command execution removed - semicolon not supported in this minishell */
void	run_list_cmd(struct s_cmd *cmd)
{
	(void)cmd;
}

void	runcmd(struct s_cmd *cmd)
{
	if (cmd == 0)
		clean_exit(0);
	if (cmd->type == EXEC)
		run_exec_cmd(cmd);
	else if (cmd->type == REDIR)
		run_redir_cmd(cmd);
	else if (cmd->type == HEREDOC)
		run_heredoc_cmd(cmd);
	/* List command handling removed - semicolon not supported in this minishell */
	else if (cmd->type == PIPE)
		run_pipe_cmd(cmd);
	else if (cmd->type == BACK)
		run_back_cmd(cmd);
	else
	{
		ft_fprintf_stderr("unknown command type: %d\n", cmd->type);
		clean_exit(1);
	}
	// Don't call clean_exit(0) here - let the calling function handle exit
}
