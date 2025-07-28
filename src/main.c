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

t_sig	g_sig = {0, 0, 0, 0};

static void	sigint_handler(int signo)
{
	(void)signo;
	if (g_sig.pid == 0)
	{
		write(STDERR_FILENO, "\n", 1);
		display_prompt();
		g_sig.exit_status = 1;
	}
	else
	{
		write(STDERR_FILENO, "\n", 1);
		g_sig.exit_status = 130;
	}
	g_sig.sigint = 1;
}

static void	sigquit_handler(int signo)
{
	(void)signo;
	if (g_sig.pid != 0)
	{
		write(STDERR_FILENO, "Quit: 3\n", 8);
		g_sig.exit_status = 131;
		g_sig.sigquit = 1;
	}
	else
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
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

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		init_signals();
		if (handle_line_input(&line))
			continue ;
		if (handle_tokenize(line, &cmd))
			continue ;
		if (handle_builtin_cmd(cmd, line))
			continue ;
		execute_cmd(cmd);
		free(line);
	}
	return (g_last_exit_status);
}
