/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**create_default_environ(void)
{
	char	**new_environ;

	new_environ = malloc(2 * sizeof(char *));
	if (! new_environ)
		return (NULL);
	new_environ[0] = malloc(ft_strlen("SHLVL=1") + 1);
	if (! new_environ[0])
	{
		free(new_environ);
		return (NULL);
	}
	ft_strcpy(new_environ[0], "SHLVL=1");
	new_environ[1] = NULL;
	return (new_environ);
}

static char	**create_environ_copy(char **envp, int count)
{
	char	**new_environ;

	new_environ = malloc((count + 1) * sizeof(char *));
	if (! new_environ)
		return (NULL);
	if (! copy_env_vars(envp, new_environ))
	{
		free(new_environ);
		return (NULL);
	}
	return (new_environ);
}

char	**copy_environ(char **envp)
{
	char	**new_environ;
	int		count;

	if (! envp)
		return (create_default_environ());
	count = count_envp_vars(envp);
	new_environ = create_environ_copy(envp, count);
	if (! new_environ)
		return (NULL);
	increment_shlvl(&new_environ);
	return (new_environ);
}

void	free_environ_copy(char **environ_copy)
{
	int	i;

	if (! environ_copy)
		return ;
	i = 0;
	while (environ_copy[i])
	{
		if (environ_copy[i])
		{
			free(environ_copy[i]);
			environ_copy[i] = NULL;
		}
		i++;
	}
	free(environ_copy);
}

void	clean_exit(int status)
{
	rl_clear_history();
	exit(status);
}
