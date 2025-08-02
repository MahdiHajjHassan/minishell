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

static int	setup_heredoc_pipe(int *pipe_fd, int *saved_stdin)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin < 0)
	{
		perror("dup failed");
		set_exit_status(1);
		return (1);
	}
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe failed");
		close(*saved_stdin);
		set_exit_status(1);
		return (1);
	}
	return (0);
}

static int	create_heredoc_child(int *pipe_fd, struct s_heredoccmd *hcmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		set_exit_status(1);
		return (-1);
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
	return (pid);
}

static int	check_heredoc_child_status(int pid, int *pipe_fd, int saved_stdin)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		close(saved_stdin);
		set_exit_status(WEXITSTATUS(status));
		return (1);
	}
	return (0);
}

static int	setup_stdin_redirection(int *pipe_fd, int saved_stdin)
{
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
	{
		perror("dup2 failed");
		close(pipe_fd[0]);
		close(saved_stdin);
		set_exit_status(1);
		return (1);
	}
	close(pipe_fd[0]);
	return (0);
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
