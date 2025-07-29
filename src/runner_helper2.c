/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_path_segment_len(char *curr, char **next)
{
	size_t	len;

	*next = strchr(curr, ':');
	if (*next)
		len = (size_t)(*next - curr);
	else
		len = strlen(curr);
	return (len);
}

int	build_full_path(char *full_path, char *curr,
					size_t len, const char *cmd)
{
	if (len + strlen(cmd) + 2 > 1024)
	{
		ft_fprintf_stderr("Path too long: %.*s/%s\n", (int)len, curr, cmd);
		return (1);
	}
	ft_strncpy(full_path, curr, len);
	full_path[len] = '/';
	ft_strcpy(full_path + len + 1, cmd);
	return (0);
}

void	reset_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd)
{
	int	status;

	(void)cmd;
	status = handle_builtin(ex->av);
	set_exit_status(status);
	exit(status);
}

int	open_redir_file_create(struct s_redircmd *rdir)
{
	int	fd;

	fd = open(rdir->file, rdir->mode, 0644);
	if (fd < 0)
	{
		ft_fprintf_stderr("open failed: %s: %s\n", rdir->file,
			strerror(errno));
		exit(1);
	}
	if (fd != rdir->fd)
	{
		dup2(fd, rdir->fd);
		close(fd);
	}
	return (0);
}
