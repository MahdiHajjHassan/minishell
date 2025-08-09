/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_existing_var(char ***env_copy, int i, const char *name,
		const char *value)
{
	char	*new_var;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (! new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	free((*env_copy)[i]);
	(*env_copy)[i] = new_var;
	return (0);
}

int	count_env_vars(char ***env_copy)
{
	int	i;

	i = 0;
	while ((*env_copy)[i])
		i++;
	return (i);
}

int	allocate_new_environ(char ***env_copy, char **new_var,
		char ***new_environ)
{
	int	count;

	count = count_env_vars(env_copy);
	*new_environ = malloc((count + 2) * sizeof(char *));
	if (! *new_environ)
	{
		free(*new_var);
		return (-1);
	}
	return (count);
}

void	copy_env_vars_for_export(char ***env_copy, char **new_environ,
		int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_environ[i] = ft_strdup((*env_copy)[i]);
		if (! new_environ[i])
		{
			while (i > 0)
			{
				i--;
				free(new_environ[i]);
			}
			free(new_environ);
			return ;
		}
		i++;
	}
}

void	finalize_new_environ(char **new_environ, int count,
	char *new_var,
		char ***env_copy)
{
	int	i;

	new_environ[count] = new_var;
	new_environ[count + 1] = NULL;
	i = 0;
	while ((*env_copy)[i])
	{
		free((*env_copy)[i]);
		i++;
	}
	free(*env_copy);
	*env_copy = new_environ;
}
