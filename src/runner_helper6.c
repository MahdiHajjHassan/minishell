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

static void	handle_command_error(const char *cmd)
{
	// Check if it's an absolute or relative path
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		// Check if file exists
		if (access(cmd, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((char *)cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			set_exit_status(127);
		}
		else
		{
			// Check if it's a directory
			struct stat st;
			if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd((char *)cmd, STDERR_FILENO);
				ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
				set_exit_status(126);
			}
			else
			{
				// File exists but not executable
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd((char *)cmd, STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				set_exit_status(126);
			}
		}
	}
	else
	{
		// Not found in PATH
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		set_exit_status(127);
	}
}

void	execute_external_cmd(struct s_execcmd *ex, char **env_copy)
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
		perror("fork failed");
		free(cmd_path);
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
		handle_child_process(cmd_path, ex->av, env_copy);
	else
		handle_parent_process(pid, cmd_path, &status);
	handle_exit_status(status);
}
