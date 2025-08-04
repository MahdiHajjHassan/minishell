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

static int	process_single_command(char *line, char **environ_copy)
{
	struct s_cmd	*cmd;
	int				result;

	result = process_command_line(line, &cmd, &environ_copy);
	if (result == 2)
	{
		if (environ_copy)
			free_environ_copy(environ_copy);
		clean_exit(get_exit_status());
	}
	else if (result == 1)
		return (1);
	else if (line)
		free(line);
	return (0);
}

static int	main_loop(char **environ_copy)
{
	char	*line;

	while (1)
	{
		init_signals();
		if (handle_line_input(&line))
		{
			if (environ_copy)
				free_environ_copy(environ_copy);
			clean_exit(get_exit_status());
		}
		if (! line || ft_strlen(line) == 0 || is_only_whitespace(line))
		{
			if (line)
				free(line);
			continue ;
		}
		process_single_command(line, environ_copy);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char			**environ_copy;

	if (validate_arguments(argc, argv))
		return (1);
	if (initialize_environment(envp, &environ_copy))
		return (1);
	setup_signals_interactive();
	rl_catch_signals = 0;
	main_loop(environ_copy);
	if (environ_copy)
		free_environ_copy(environ_copy);
	clean_exit(0);
	return (0);
}
