/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler_interactive(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_exit_status(130);
}

static void	sigquit_handler_interactive(int signo)
{
	(void)signo;
}

static void	sigint_handler_noninteractive(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
}

static void	sigquit_handler_noninteractive(int signo)
{
	(void)signo;
}

static void	setup_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler_interactive;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

static void	setup_signals_noninteractive(void)
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

static int	validate_arguments(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	initialize_environment(char **envp, char ***environ_copy)
{
	*environ_copy = copy_environ(envp);
	if (! *environ_copy)
	{
		ft_putstr_fd("Failed to copy environment\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	process_builtin_command(struct s_cmd *cmd, char *line,
		char ***environ_copy)
{
	if (handle_builtin_cmd(cmd, line, environ_copy))
	{
		free_cmd(cmd);
		if (line)
			free(line);
		return (1);
	}
	return (0);
}

static void	execute_external_command(struct s_cmd *cmd, char **environ_copy)
{
	setup_signals_noninteractive();
	execute_cmd(cmd, environ_copy);
	setup_signals_interactive();
	free_cmd(cmd);
}

static int	process_command_line(char *line, struct s_cmd **cmd,
		char ***environ_copy)
{
	if (handle_tokenize(line, cmd, *environ_copy))
		return (1);
	if (*cmd)
	{
		if (process_builtin_command(*cmd, line, environ_copy))
			return (1);
		execute_external_command(*cmd, *environ_copy);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	struct s_cmd	*cmd;
	char			**environ_copy;

	if (validate_arguments(argc, argv))
		return (1);
	if (initialize_environment(envp, &environ_copy))
		return (1);
	setup_signals_interactive();
	rl_catch_signals = 0;
	while (1)
	{
		init_signals();
		if (handle_line_input(&line))
			continue ;
		if (! line || ft_strlen(line) == 0 || is_only_whitespace(line))
			continue ;
		if (process_command_line(line, &cmd, &environ_copy))
			continue ;
		if (line)
			free(line);
	}
	clean_exit(0);
	return (0);
}
