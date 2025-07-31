/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle SHLVL increment when shell starts */
static void	increment_shlvl(char ***env_copy)
{
	char	*shlvl_value;
	int		shlvl_num;
	char	*new_shlvl;
	int		i;

	/* Find existing SHLVL */
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], "SHLVL=", 6) == 0)
		{
			shlvl_value = (*env_copy)[i] + 6;
			shlvl_num = ft_atoi(shlvl_value);
			
			/* Handle invalid or too large values */
			if (shlvl_num < 0 || shlvl_num >= 1000)
				shlvl_num = 0;
			
			shlvl_num++;
			new_shlvl = ft_itoa(shlvl_num);
			if (new_shlvl)
			{
				free((*env_copy)[i]);
				(*env_copy)[i] = malloc(ft_strlen("SHLVL=") + ft_strlen(new_shlvl) + 1);
				if ((*env_copy)[i])
				{
					ft_strcpy((*env_copy)[i], "SHLVL=");
					ft_strlcat((*env_copy)[i], new_shlvl, ft_strlen("SHLVL=") + ft_strlen(new_shlvl) + 1);
				}
				free(new_shlvl);
			}
			return ;
		}
		i++;
	}
	
	/* SHLVL doesn't exist, add it with value 1 */
	new_shlvl = ft_itoa(1);
	if (new_shlvl)
	{
		ft_setenv("SHLVL", new_shlvl, 1, env_copy);
		free(new_shlvl);
	}
}

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
	
	/* Handle SHLVL increment */
	increment_shlvl(&new_environ);
	
	return (new_environ);
}

/* Free environment copy on exit */
void	free_environ_copy(char **environ_copy)
{
	int	i;

	if (!environ_copy)
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

/* Clean exit function */
void	clean_exit(int status)
{
	/* Don't free global environ - we work with our local copy */
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