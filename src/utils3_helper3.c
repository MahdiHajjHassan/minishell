/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_total_vars(char ***env_copy)
{
	int	total_vars;

	total_vars = 0;
	while ((*env_copy)[total_vars])
		total_vars++;
	return (total_vars);
}

void	copy_vars_except_index(char ***env_copy, char **new_environ,
		int var_index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*env_copy)[i])
	{
		if (i != var_index)
		{
			new_environ[j] = (*env_copy)[i];
			j++;
		}
		i++;
	}
	new_environ[j] = NULL;
}

int	remove_var_from_env(char ***env_copy, int var_index)
{
	char	**new_environ;
	int		total_vars;

	total_vars = count_total_vars(env_copy);
	new_environ = malloc((total_vars) * sizeof(char *));
	if (! new_environ)
		return (-1);
	copy_vars_except_index(env_copy, new_environ, var_index);
	free((*env_copy)[var_index]);
	free(*env_copy);
	*env_copy = new_environ;
	return (0);
}

int	find_var_index(char ***env_copy, const char *name, int name_len)
{
	int		i;

	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0
			&& (*env_copy)[i][name_len] == '=')
		{
			return (i);
		}
		i++;
	}
	return (-1);
}
