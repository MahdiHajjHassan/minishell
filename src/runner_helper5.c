/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_error(struct s_redircmd *rdir, int saved_stdin,
		int saved_stdout)
{
	set_exit_status(1);
	if (rdir->fd == STDIN_FILENO)
		restore_stdin(saved_stdin);
	else if (rdir->fd == STDOUT_FILENO)
		restore_stdout(saved_stdout);
}

int	setup_redirection(struct s_redircmd *rdir, int *saved_stdin,
		int *saved_stdout)
{
	if (rdir->fd == STDIN_FILENO)
		*saved_stdin = dup(STDIN_FILENO);
	else if (rdir->fd == STDOUT_FILENO)
		*saved_stdout = dup(STDOUT_FILENO);
	close(rdir->fd);
	return (0);
}

int	open_redirection_file(struct s_redircmd *rdir, int saved_stdin,
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

void	restore_redirection(struct s_redircmd *rdir, int saved_stdin,
		int saved_stdout)
{
	if (rdir->fd == STDIN_FILENO)
		restore_stdin(saved_stdin);
	else if (rdir->fd == STDOUT_FILENO)
		restore_stdout(saved_stdout);
}
