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

/* Copy environment variables from envp to a new array */
char	**copy_environ(char **envp)
{
	char	**new_environ;
	int		i;
	int		count;

	if (!envp)
		return (NULL);
	
	/* Count environment variables */
	count = 0;
	while (envp[count])
		count++;
	
	/* Allocate new environment array */
	new_environ = malloc((count + 1) * sizeof(char *));
	if (!new_environ)
		return (NULL);
	
	/* Copy each environment variable */
	i = 0;
	while (envp[i])
	{
		new_environ[i] = ft_strdup(envp[i]);
		if (!new_environ[i])
		{
			/* Clean up on failure */
			while (i > 0)
			{
				i--;
				free(new_environ[i]);
			}
			free(new_environ);
			return (NULL);
		}
		i++;
	}
	new_environ[i] = NULL;
	return (new_environ);
}

/* Free environment copy on exit */
void	free_environ_copy(char **environ_copy)
{
	int	i;

	i = 0;
	if (environ_copy)
	{
		while (environ_copy[i])
		{
			free(environ_copy[i]);
			i++;
		}
		free(environ_copy);
	}
}

/* Clean exit function */
void	clean_exit(int status)
{
	extern char	**environ;
	free_environ_copy(environ);
	environ = NULL;
	rl_clear_history();
	exit(status);
}

/* Memory leak check helper for debugging */
#ifdef DEBUG
void	check_leaks(void)
{
	system("leaks minishell");
}
#endif 