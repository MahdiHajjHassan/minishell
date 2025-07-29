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
		exit(0);
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
		open_redir_file_create(rdir);
	}
	else
	{
		open_redir_file_regular(rdir);
	}
	runcmd(rdir->cmd);
}

void	run_list_cmd(struct s_cmd *cmd)
{
	struct s_listcmd	*list;

	list = (struct s_listcmd *)cmd;
	process_list_left(list);
	process_list_right(list);
}

void	runcmd(struct s_cmd *cmd)
{
	if (cmd == 0)
		exit(0);
	if (cmd->type == EXEC)
		run_exec_cmd(cmd);
	else if (cmd->type == REDIR)
		run_redir_cmd(cmd);
	else if (cmd->type == LIST)
	{
		run_list_cmd(cmd);
		return ;
	}
	else if (cmd->type == PIPE)
		run_pipe_cmd(cmd);
	else if (cmd->type == BACK)
		run_back_cmd(cmd);
	else
	{
		ft_fprintf_stderr("unknown command type: %d\n", cmd->type);
		exit(1);
	}
	if (cmd->type != LIST)
		exit(0);
}
