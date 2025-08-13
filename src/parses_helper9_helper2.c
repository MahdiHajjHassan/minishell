/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper9_helper2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*find_innermost_cmd_for_fd(struct s_cmd *cmd, int fd)
{
	struct s_cmd		*innermost;
	struct s_redircmd	*inner_rdir;

	innermost = cmd;
	while (innermost && innermost->type == REDIR)
	{
		inner_rdir = (struct s_redircmd *)innermost;
		if (inner_rdir->fd != fd)
			break ;
		innermost = inner_rdir->cmd;
	}
	return (innermost);
}

void	update_redirection_chain_for_fd(struct s_cmd *cmd,
		struct s_cmd *innermost, struct s_cmd *new_rdir, int fd)
{
	struct s_cmd		*current;
	struct s_redircmd	*current_rdir;

	current = cmd;
	while (current && current->type == REDIR)
	{
		current_rdir = (struct s_redircmd *)current;
		if (current_rdir->fd != fd)
			break ;
		if (current_rdir->cmd == innermost)
		{
			current_rdir->cmd = new_rdir;
			break ;
		}
		current = current_rdir->cmd;
	}
}

struct s_cmd	*replace_input_redir(struct s_cmd *cmd, char *file)
{
	struct s_redircmd	*rdir;

	if (!cmd || cmd->type != REDIR)
		return (apply_input_redir(cmd, file));
	rdir = (struct s_redircmd *)cmd;
	rdir->cmd = replace_input_redir(rdir->cmd, file);
	return (cmd);
}

struct s_cmd	*find_innermost_cmd(struct s_cmd *cmd)
{
	struct s_cmd		*innermost;
	struct s_redircmd	*inner_rdir;

	innermost = cmd;
	while (innermost && innermost->type == REDIR)
	{
		inner_rdir = (struct s_redircmd *)innermost;
		if (inner_rdir->fd != STDOUT_FILENO)
			break ;
		innermost = inner_rdir->cmd;
	}
	return (innermost);
}

void	update_redirection_chain(struct s_cmd *cmd, struct s_cmd *innermost,
		struct s_cmd *new_rdir)
{
	struct s_cmd		*current;
	struct s_redircmd	*current_rdir;

	current = cmd;
	while (current && current->type == REDIR)
	{
		current_rdir = (struct s_redircmd *)current;
		if (current_rdir->fd != STDOUT_FILENO)
			break ;
		if (current_rdir->cmd == innermost)
		{
			current_rdir->cmd = new_rdir;
			break ;
		}
		current = current_rdir->cmd;
	}
}
