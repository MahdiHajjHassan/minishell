/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management_helper.c                          :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_shlvl_value(char ***env_copy, int i, int shlvl_num)
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

void	create_new_shlvl_helper(char **new_environ, int count, char *new_shlvl)
{
	new_environ[count] = malloc(ft_strlen("SHLVL=") + ft_strlen(new_shlvl) + 1);
	if (new_environ[count])
	{
		ft_strcpy(new_environ[count], "SHLVL=");
		ft_strlcat(new_environ[count], new_shlvl, ft_strlen("SHLVL=")
			+ ft_strlen(new_shlvl) + 1);
	}
	new_environ[count + 1] = NULL;
}

void	copy_env_array(char **new_environ, char **env_copy, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_environ[i] = env_copy[i];
		i++;
	}
}

void	create_new_shlvl(char ***env_copy)
{
	char	*new_shlvl;
	char	**new_environ;
	int		count;

	new_shlvl = ft_itoa(1);
	if (! new_shlvl)
		return ;
	count = 0;
	while ((*env_copy)[count])
		count++;
	new_environ = malloc((count + 2) * sizeof(char *));
	if (! new_environ)
	{
		free(new_shlvl);
		return ;
	}
	copy_env_array(new_environ, *env_copy, count);
	create_new_shlvl_helper(new_environ, count, new_shlvl);
	free(new_shlvl);
	*env_copy = new_environ;
}

void	increment_shlvl(char ***env_copy)
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
			shlvl_num = atoi(shlvl_value);
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
