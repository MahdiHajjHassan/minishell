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

	i = 0;
	while (ecmd->av[i])
	{
		original = ecmd->av[i];
		ecmd->av[i] = expand_variables(original, ft_strlen(original), env_copy);
		free(original);
		i++;
	}
}

int	handle_builtin_cmd(struct s_cmd *cmd, char *line, char ***env_copy)
{
	struct s_execcmd	*ecmd;
	int					status;

	(void)line; /* Parameter not used in this function */
	if (cmd->type == EXEC)
	{
		ecmd = (struct s_execcmd *)cmd;
		if (ecmd->av[0] && is_builtin(ecmd->av[0]))
		{
			/* Variable expansion happens during parsing with quote context awareness */
			status = handle_builtin(ecmd->av, env_copy);
			set_exit_status(status);
			/* Don't free cmd and line here - let the main loop handle it */
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
