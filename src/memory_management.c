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

static void	update_shlvl_value(char ***env_copy, int i, int shlvl_num)
{
	char	*new_shlvl;
	char	*old_value;

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
}

static void	create_new_shlvl(char ***env_copy)
{
	char	*new_shlvl;

	new_shlvl = ft_itoa(1);
	if (new_shlvl)
	{
		ft_setenv("SHLVL", new_shlvl, 1, env_copy);
		free(new_shlvl);
	}
}

static void	increment_shlvl(char ***env_copy)
{
	char	*shlvl_value;
	int		shlvl_num;
	int		i;

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
			update_shlvl_value(env_copy, i, shlvl_num);
			return ;
		}
		i++;
	}
	create_new_shlvl(env_copy);
}

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static void	cleanup_on_failure(char **new_environ, int i)
{
	while (i > 0)
	{
		i--;
		free(new_environ[i]);
	}
	free(new_environ);
}

static int	copy_env_vars(char **envp, char **new_environ)
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

char	**copy_environ(char **envp)
{
	char	**new_environ;
	int		count;

	if (! envp)
		return (NULL);
	count = count_env_vars(envp);
	new_environ = malloc((count + 1) * sizeof(char *));
	if (! new_environ)
		return (NULL);
	if (! copy_env_vars(envp, new_environ))
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
