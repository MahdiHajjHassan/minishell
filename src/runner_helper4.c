/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_left(int *p, struct s_pipecmd *pipecmd, char **env_copy)
{
	(void)p;
	(void)pipecmd;
	(void)env_copy;
}

void	setup_pipe_right(int *p, struct s_pipecmd *pipecmd, char **env_copy)
{
	(void)p;
	(void)pipecmd;
	(void)env_copy;
}

void	run_heredoc_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_heredoccmd	*hcmd;
	int					pipe_fd[2];
	pid_t				pid;
	int					saved_stdin;

	hcmd = (struct s_heredoccmd *)cmd;
	if (setup_heredoc_pipe(pipe_fd, &saved_stdin))
		return ;
	pid = create_heredoc_child(pipe_fd, hcmd);
	if (pid < 0)
	{
		close(saved_stdin);
		return ;
	}
	if (check_heredoc_child_status(pid, pipe_fd, saved_stdin))
		return ;
	if (setup_stdin_redirection(pipe_fd, saved_stdin))
		return ;
	runcmd(hcmd->cmd, env_copy);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}
