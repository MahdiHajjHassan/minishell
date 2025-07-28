/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_healper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_builtin_args(struct s_execcmd *ecmd)
{
	for (int i = 0; ecmd->av[i]; i++)
	{
		char	*original;

		original = ecmd->av[i];
		ecmd->av[i] = expand_variables(original, strlen(original));
		free(original);
	}
}

int	handle_builtin_cmd(struct s_cmd *cmd, char *line)
{
	struct s_execcmd	*ecmd;
	int					status;

	if (cmd->type == EXEC)
	{
		ecmd = (struct s_execcmd *)cmd;
		if (ecmd->av[0] && is_builtin(ecmd->av[0]))
		{
			expand_builtin_args(ecmd);
			status = handle_builtin(ecmd->av);
			set_exit_status(status);
			free(line);
			return (1);
		}
	}
	return (0);
}

void	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		set_exit_status(128 + WTERMSIG(status));
	}
	else
	{
		set_exit_status(WEXITSTATUS(status));
	}
}
