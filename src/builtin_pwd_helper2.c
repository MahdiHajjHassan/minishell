/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_pwd_variables(t_pwd_prep_params params)
{
	int		count;

	*params.old_pwd_var = create_old_pwd_var(params.old_pwd);
	*params.new_pwd_var = create_new_pwd_var(params.new_pwd);
	if (! *params.old_pwd_var || ! *params.new_pwd_var)
	{
		free(*params.old_pwd_var);
		free(*params.new_pwd_var);
		return (0);
	}
	count = count_env_vars_pwd(params.env_copy);
	*params.new_environ = allocate_new_environ_pwd(params.env_copy, count);
	if (! *params.new_environ)
	{
		free(*params.old_pwd_var);
		free(*params.new_pwd_var);
		return (0);
	}
	return (count);
}

void	finalize_new_environ_pwd(char **new_environ, int count,
		char *old_pwd_var, char *new_pwd_var)
{
	int		i;
	int		oldpwd_found;
	int		pwd_found;

	(void)old_pwd_var;
	(void)new_pwd_var;
	oldpwd_found = 0;
	pwd_found = 0;
	i = 0;
	while (i < count)
	{
		if (new_environ[i] == old_pwd_var)
			oldpwd_found = 1;
		if (new_environ[i] == new_pwd_var)
			pwd_found = 1;
		i++;
	}
	i = count;
	if (!oldpwd_found)
		i++;
	if (!pwd_found)
		i++;
	new_environ[i] = NULL;
}
