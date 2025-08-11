/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper6.c                                      :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_command_line(char *line, struct s_cmd **cmd,
		char ***environ_copy)
{
	int	result;

	if (handle_tokenize(line, cmd, *environ_copy))
		return (1);
	if (*cmd)
	{
		result = process_builtin_command(*cmd, line, environ_copy);
		if (result)
			return (result);
		execute_external_command(*cmd, *environ_copy);
	}
	return (0);
}

void	cleanup_and_exit(struct s_cmd *cmd, char ***environ_copy)
{
	if (cmd)
		free_cmd(cmd);
	if (*environ_copy)
		free_environ_copy(*environ_copy);
	clean_exit(get_exit_status());
}

int	cleanup_and_return(struct s_cmd *cmd)
{
	if (cmd)
		free_cmd(cmd);
	return (1);
}

void	cleanup_and_free_line(struct s_cmd *cmd, char *line)
{
	if (cmd)
		free_cmd(cmd);
	if (line)
		free(line);
}

int	process_single_command(char *line, char ***environ_copy)
{
	struct s_cmd	*cmd;
	int				result;

	result = process_command_line(line, &cmd, environ_copy);
	if (result == 2)
	{
		cleanup_and_exit(cmd, environ_copy);
	}
	else if (result == 1)
	{
		return (cleanup_and_return(cmd));
	}
	else
	{
		cleanup_and_free_line(cmd, line);
	}
	return (0);
}
