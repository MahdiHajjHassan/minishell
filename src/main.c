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
