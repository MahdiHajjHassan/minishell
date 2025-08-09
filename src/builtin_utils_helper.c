/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_existing_env_vars(t_env_update_params params)
{
	int	i;

	i = 0;
	while ((*params.env_copy)[i])
	{
		if (ft_strncmp((*params.env_copy)[i], "OLDPWD=", 7) == 0)
		{
			params.new_environ[i] = params.old_pwd_var;
			params.flags[0] = 1;
		}
		else if (ft_strncmp((*params.env_copy)[i], "PWD=", 4) == 0)
		{
			params.new_environ[i] = params.new_pwd_var;
			params.flags[1] = 1;
		}
		else
		{
			params.new_environ[i] = ft_strdup((*params.env_copy)[i]);
		}
		i++;
	}
}

void	add_missing_env_vars(char **new_environ, char *old_pwd_var,
		char *new_pwd_var, int *flags)
{
	int	i;
	int	count;

	count = 0;
	while (new_environ[count])
		count++;
	i = count;
	if (!flags[0])
	{
		new_environ[i] = old_pwd_var;
		i++;
	}
	if (!flags[1])
	{
		new_environ[i] = new_pwd_var;
		i++;
	}
}
