/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
