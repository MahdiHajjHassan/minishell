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
	int					status;
	int					saved_stdin;

	hcmd = (struct s_heredoccmd *)cmd;
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
	{
		perror("dup failed");
		set_exit_status(1);
		return ;
	}
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe failed");
		close(saved_stdin);
		set_exit_status(1);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		close(saved_stdin);
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (hcmd->content && ft_strlen(hcmd->content) > 0)
		{
			write(pipe_fd[1], hcmd->content, ft_strlen(hcmd->content));
		}
		close(pipe_fd[1]);
		clean_exit(0);
	}
	else
	{
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			close(pipe_fd[0]);
			close(saved_stdin);
			set_exit_status(WEXITSTATUS(status));
			return ;
		}
		if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 failed");
			close(pipe_fd[0]);
			close(saved_stdin);
			set_exit_status(1);
			return ;
		}
		close(pipe_fd[0]);
		runcmd(hcmd->cmd, env_copy);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
}
