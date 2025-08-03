/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management_helper2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_envp_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

void	cleanup_on_failure(char **new_environ, int i)
{
	while (i > 0)
	{
		i--;
		free(new_environ[i]);
	}
	free(new_environ);
}

int	copy_env_vars(char **envp, char **new_environ)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		new_environ[i] = ft_strdup(envp[i]);
		if (! new_environ[i])
		{
			cleanup_on_failure(new_environ, i);
			return (0);
		}
		i++;
	}
	new_environ[i] = NULL;
	return (1);
}
