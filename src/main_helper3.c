/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_builtin_args(struct s_execcmd *ecmd, char **env_copy)
{
	int		i;
	char	*original;
	char	*expanded;

	i = 0;
	while (ecmd->av[i])
	{
		original = ecmd->av[i];
		expanded = expand_variables(original, ft_strlen(original),
				env_copy);
		if (expanded)
		{
			ecmd->av[i] = expanded;
			free(original);
		}
		i++;
	}
}

int	handle_builtin_cmd(struct s_cmd *cmd, char *line, char ***env_copy)
{
	struct s_execcmd	*ecmd;
	int					status;

	(void)line;
	if (cmd->type == EXEC)
	{
		ecmd = (struct s_execcmd *)cmd;
		if (ecmd->av[0] && is_builtin(ecmd->av[0]))
		{
			status = handle_builtin(ecmd->av, env_copy);
			if (ecmd->av[0] && !ft_strcmp(ecmd->av[0], "exit"))
			{
				set_exit_status(status);
				if (status == 1)
					return (1);
				return (2);
			}
			set_exit_status(status);
			return (1);
		}
	}
	return (0);
}

void	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			set_exit_status(130);
		else if (WTERMSIG(status) == SIGQUIT)
			set_exit_status(131);
		else
			set_exit_status(128 + WTERMSIG(status));
	}
	else
	{
		set_exit_status(WEXITSTATUS(status));
	}
}
