/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals_noninteractive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler_noninteractive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler_noninteractive;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	validate_arguments(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	initialize_environment(char **envp, char ***environ_copy)
{
	*environ_copy = copy_environ(envp);
	if (! *environ_copy)
	{
		ft_putstr_fd("Failed to copy environment\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	process_builtin_command(struct s_cmd *cmd, char *line,
		char ***environ_copy)
{
	int	result;

	result = handle_builtin_cmd(cmd, line, environ_copy);
	if (result == 2)
	{
		free_cmd(cmd);
		if (line)
			free(line);
		return (result);
	}
	else if (result == 1)
	{
		if (line)
			free(line);
		return (result);
	}
	return (0);
}

void	execute_external_command(struct s_cmd *cmd, char **environ_copy)
{
	setup_signals_noninteractive();
	execute_cmd(cmd, environ_copy);
	setup_signals_interactive();
	free_cmd(cmd);
}
