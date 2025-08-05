/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper6_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_fork_error(char *cmd_path)
{
	perror("fork failed");
	free(cmd_path);
	set_exit_status(1);
}

void	execute_command_process(struct s_execcmd *ex, char **env_copy)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

	cmd_path = find_command(ex->av[0], env_copy);
	if (! cmd_path)
	{
		handle_command_error(ex->av[0]);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		handle_fork_error(cmd_path);
		return ;
	}
	if (pid == 0)
	{
		handle_child_process(cmd_path, ex->av, env_copy);
		return ;
	}
	else
		handle_parent_process(pid, cmd_path, &status);
	handle_exit_status(status);
}
