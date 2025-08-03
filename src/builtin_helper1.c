/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_env_to_sorted_array(char **env_copy, int count)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc((count + 1) * sizeof(char *));
	if (! sorted_env)
		return (NULL);
	i = 0;
	while (env_copy && env_copy[i])
	{
		sorted_env[i] = ft_strdup(env_copy[i]);
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}

static void	sort_env_array(char **sorted_env)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (sorted_env[i])
	{
		j = i + 1;
		while (sorted_env[j])
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				temp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_array(char **sorted_env)
{
	int		i;

	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		format_export_output(sorted_env[i]);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

static void	free_sorted_array(char **sorted_env)
{
	int		i;

	i = 0;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

void	print_sorted_env_vars(char **env_copy)
{
	int		count;
	char	**sorted_env;

	count = 0;
	while (env_copy && env_copy[count])
		count++;
	sorted_env = copy_env_to_sorted_array(env_copy, count);
	if (! sorted_env)
		return ;
	sort_env_array(sorted_env);
	print_sorted_array(sorted_env);
	free_sorted_array(sorted_env);
}
