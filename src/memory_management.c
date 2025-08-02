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

static void	increment_shlvl(char ***env_copy)
{
	char	*shlvl_value;
	int		shlvl_num;
	char	*new_shlvl;
	int		i;
	char	*old_value;

	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], "SHLVL=", 6) == 0)
		{
			shlvl_value = (*env_copy)[i] + 6;
			shlvl_num = ft_atoi(shlvl_value);
			if (shlvl_num < 0 || shlvl_num >= 1000)
				shlvl_num = 0;
			shlvl_num++;
			new_shlvl = ft_itoa(shlvl_num);
			if (new_shlvl)
			{
				old_value = (*env_copy)[i];
				(*env_copy)[i] = malloc(ft_strlen("SHLVL=")
					+ ft_strlen(new_shlvl) + 1);
				if ((*env_copy)[i])
				{
					ft_strcpy((*env_copy)[i], "SHLVL=");
					ft_strlcat((*env_copy)[i], new_shlvl,
						ft_strlen("SHLVL=") + ft_strlen(new_shlvl) + 1);
					free(old_value);
				}
				else
				{
					(*env_copy)[i] = old_value;
				}
				free(new_shlvl);
			}
			return ;
		}
		i++;
	}
	new_shlvl = ft_itoa(1);
	if (new_shlvl)
	{
		ft_setenv("SHLVL", new_shlvl, 1, env_copy);
		free(new_shlvl);
	}
}

char	**copy_environ(char **envp)
{
	char	**new_environ;
	int		i;
	int		count;

	if (! envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	new_environ = malloc((count + 1) * sizeof(char *));
	if (! new_environ)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_environ[i] = ft_strdup(envp[i]);
		if (! new_environ[i])
		{
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

#ifdef DEBUG
void	check_leaks(void)
{
	system("leaks minishell");
}
#endif 