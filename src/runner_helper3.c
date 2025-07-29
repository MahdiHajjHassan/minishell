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
		ft_fprintf_stderr("open failed: %s: %s\n", rdir->file,
			strerror(errno));
		exit(1);
	}
}

/* List command functions removed - semicolon not supported in this minishell */
void	handle_list_builtin(struct s_execcmd *ex)
{
	(void)ex;
}

void	handle_list_external(struct s_cmd *cmd)
{
	(void)cmd;
}

void	process_list_left(struct s_listcmd *list)
{
	(void)list;
}

void	process_list_right(struct s_listcmd *list)
{
	(void)list;
}
