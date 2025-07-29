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

void	setup_pipe_left(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->left);
	}
}

void	setup_pipe_right(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->right);
	}
}

void	run_back_cmd(struct s_cmd *cmd)
{
	struct s_backcmd	*back;

	back = (struct s_backcmd *)cmd;
	if (forkk() == 0)
	{
		if (forkk() == 0)
		{
			runcmd(back->cmd);
		}
		exit(0);
	}
	wait(0);
}

void	run_heredoc_cmd(struct s_cmd *cmd)
{
	struct s_heredoccmd	*hcmd;
	int					pipe_fd[2];
	pid_t				pid;
	int					status;

	hcmd = (struct s_heredoccmd *)cmd;
	
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe failed");
		exit(1);
	}
	
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	
	if (pid == 0)
	{
		close(pipe_fd[0]);
		
		if (hcmd->content && ft_strlen(hcmd->content) > 0)
		{
			write(pipe_fd[1], hcmd->content, ft_strlen(hcmd->content));
		}
		close(pipe_fd[1]);
		exit(0);
	}
	else
	{
		close(pipe_fd[1]);
		
		waitpid(pid, &status, 0);
		
		if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 failed");
			exit(1);
		}
		close(pipe_fd[0]);
		
		runcmd(hcmd->cmd);
	}
}
