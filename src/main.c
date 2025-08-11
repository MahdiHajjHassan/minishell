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

static int	main_loop(char ***environ_copy)
{
	char	*line;

	while (1)
	{
		if (handle_line_input(&line))
		{
			if (*environ_copy)
				free_environ_copy(*environ_copy);
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
	main_loop(&environ_copy);
	if (environ_copy)
		free_environ_copy(environ_copy);
	clean_exit(0);
	return (0);
}
