/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper9_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*replace_output_redir(struct s_cmd *cmd, char *file)
{
	struct s_redircmd	*rdir;
	struct s_cmd		*innermost;
	struct s_redircmd	*new_rdir;

	if (cmd && cmd->type == REDIR)
	{
		rdir = (struct s_redircmd *)cmd;
		if (rdir->fd == STDOUT_FILENO)
		{
			innermost = find_innermost_cmd(cmd);
			new_rdir = (struct s_redircmd *)redircmd(innermost, file, NULL,
					(t_redir_params){O_WRONLY | O_CREAT | O_TRUNC,
					STDOUT_FILENO});
			if (cmd == innermost)
				return ((struct s_cmd *)new_rdir);
			update_redirection_chain(cmd, innermost, (struct s_cmd *)new_rdir);
			return (cmd);
		}
	}
	return (apply_output_redir(cmd, file));
}

struct s_cmd	*replace_append_redir(struct s_cmd *cmd, char *file)
{
	struct s_redircmd	*rdir;
	struct s_cmd		*innermost;
	struct s_redircmd	*new_rdir;

	if (cmd && cmd->type == REDIR)
	{
		rdir = (struct s_redircmd *)cmd;
		if (rdir->fd == STDOUT_FILENO)
		{
			innermost = find_innermost_cmd(cmd);
			new_rdir = (struct s_redircmd *)redircmd(innermost, file, NULL,
					(t_redir_params){O_WRONLY | O_CREAT | O_APPEND,
					STDOUT_FILENO});
			if (cmd == innermost)
				return ((struct s_cmd *)new_rdir);
			update_redirection_chain(cmd, innermost, (struct s_cmd *)new_rdir);
			return (cmd);
		}
	}
	return (apply_append_redir(cmd, file));
}
