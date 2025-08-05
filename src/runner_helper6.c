/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(char *cmd_path, char **av, char **env_copy)
{
	reset_signals();
	execve(cmd_path, av, env_copy);
	perror("execve failed");
	free(cmd_path);
	clean_exit(1);
}

void	handle_parent_process(pid_t pid, char *cmd_path, int *status)
{
	free(cmd_path);
	waitpid(pid, status, 0);
}

void	handle_exit_status(int status)
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

void	handle_command_error(const char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
		handle_absolute_path_error(cmd);
	else
		print_command_error(cmd, ": command not found\n", 127);
}

void	execute_external_cmd(struct s_execcmd *ex, char **env_copy)
{
	execute_command_process(ex, env_copy);
}
