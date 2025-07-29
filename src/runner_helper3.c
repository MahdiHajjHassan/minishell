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

int	open_redir_file_regular(struct s_redircmd *rdir)
{
	int	fd;
	int	save_errno;

	fd = open(rdir->file, rdir->mode);
	if (fd < 0)
	{
		save_errno = errno;
		ft_fprintf_stderr("open failed: %s: %s\n", rdir->file,
			strerror(save_errno));
		return (1);
	}
	if (fd != rdir->fd)
	{
		if (dup2(fd, rdir->fd) < 0)
		{
			save_errno = errno;
			ft_fprintf_stderr("dup2 failed: %s\n", strerror(save_errno));
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
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
