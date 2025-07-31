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

volatile sig_atomic_t	g_signal = 0;

static void	sigint_handler(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigquit_handler(int signo)
{
	(void)signo;
	/* Do nothing in interactive mode as required by subject */
}

static void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	struct s_cmd	*cmd;
	char			**environ_copy;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	
	/* Copy environment */
	environ_copy = copy_environ(envp);
	if (!environ_copy)
	{
		ft_putstr_fd("Failed to copy environment\n", STDERR_FILENO);
		return (1);
	}
	/* Don't set global environ - we work with our local copy */
	
	setup_signals();
	rl_catch_signals = 0;
	
	while (1)
	{
		init_signals();
		if (handle_line_input(&line))
			continue ;
		if (handle_tokenize(line, &cmd, environ_copy))
			continue ;
		if (cmd)
		{
			if (handle_builtin_cmd(cmd, line, &environ_copy))
			{
				free_cmd(cmd);
				if (line)
					free(line);
				continue ;
			}
			execute_cmd(cmd, environ_copy);
			free_cmd(cmd);
		}
		if (line)
			free(line);
	}
	
	/* Clean up on normal exit */
	clean_exit(0);
	return (0);
}
