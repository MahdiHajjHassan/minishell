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

void	print_sorted_env_vars(char **env_copy)
{
	int		count;
	char	**sorted_env;

	count = count_env(env_copy);
	sorted_env = alloc_and_copy_env(env_copy, count);
	if (!sorted_env)
		return ;
	sort_env(sorted_env);
	print_and_free_sorted_env(sorted_env);
}

static int	count_env(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

static char	**alloc_and_copy_env(char **env, int count)
{
	int		i;
	char	**sorted_env;

	sorted_env = malloc((count + 1) * sizeof(char *));
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		sorted_env[i] = ft_strdup(env[i]);
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_and_free_sorted_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		format_export_output(env[i]);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}
