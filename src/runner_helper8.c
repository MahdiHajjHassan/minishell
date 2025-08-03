/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_background_child(struct s_backcmd *backcmd, char **env_copy)
{
	runcmd(backcmd->cmd, env_copy);
	exit(get_exit_status());
}

void	handle_background_status(int status)
{
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			set_exit_status(130);
		else if (WTERMSIG(status) == SIGQUIT)
			set_exit_status(131);
		else
			set_exit_status(128 + WTERMSIG(status));
	}
	else
		set_exit_status(1);
}

void	run_back_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_backcmd	*backcmd;
	pid_t				pid;
	int					status;

	backcmd = (struct s_backcmd *)cmd;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
		handle_background_child(backcmd, env_copy);
	waitpid(pid, &status, 0);
	handle_background_status(status);
}
