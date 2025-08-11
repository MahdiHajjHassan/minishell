/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(int *p)
{
	if (pipe(p) < 0)
	{
		perror("pipe failed");
		set_exit_status(1);
		return (0);
	}
	return (1);
}

int	create_first_child(int *p, struct s_pipecmd *pipecmd,
	char **env_copy)
{
	pid_t	pid1;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork failed");
		close(p[0]);
		close(p[1]);
		set_exit_status(1);
		return (-1);
	}
	if (pid1 == 0)
	{
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		runcmd(pipecmd->left, env_copy);
		free_cmd(pipecmd->left);
		exit(get_exit_status());
	}
	return (pid1);
}

int	create_second_child(int *p, struct s_pipecmd *pipecmd,
	char **env_copy,
		pid_t pid1)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork failed");
		close(p[0]);
		close(p[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		set_exit_status(1);
		return (-1);
	}
	if (pid2 == 0)
	{
		close(p[1]);
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		runcmd(pipecmd->right, env_copy);
		free_cmd(pipecmd->right);
		exit(get_exit_status());
	}
	return (pid2);
}

void	run_pipe_cmd(struct s_cmd *cmd, char **env_copy)
{
	t_pipe_cmd_params	params;

	params.pipecmd = (struct s_pipecmd *)cmd;
	if (! setup_pipe(params.p))
		return ;
	params.pid1 = create_first_child(params.p, params.pipecmd, env_copy);
	if (params.pid1 == -1)
		return ;
	params.pid2 = create_second_child(params.p,
			params.pipecmd, env_copy, params.pid1);
	if (params.pid2 == -1)
		return ;
	close(params.p[0]);
	close(params.p[1]);
	wait_for_children(params.pid1, params.pid2,
		&params.status1, &params.status2);
	handle_pipe_status(params.status2);
}
