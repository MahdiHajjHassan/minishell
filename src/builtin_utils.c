/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (! ft_strcmp(cmd, "echo"))
		return (1);
	if (! ft_strcmp(cmd, "cd"))
		return (1);
	if (! ft_strcmp(cmd, "pwd"))
		return (1);
	if (! ft_strcmp(cmd, "export"))
		return (1);
	if (! ft_strcmp(cmd, "unset"))
		return (1);
	if (! ft_strcmp(cmd, "env"))
		return (1);
	if (! ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

char	*create_old_pwd_var(char *old_pwd)
{
	char	*old_pwd_var;

	old_pwd_var = malloc(ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	if (! old_pwd_var)
		return (NULL);
	ft_strcpy(old_pwd_var, "OLDPWD=");
	ft_strlcat(old_pwd_var, old_pwd,
		ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	return (old_pwd_var);
}

char	**allocate_new_environ_pwd(char ***env_copy, int count)
{
	char	**new_environ;
	int		k;

	(void)env_copy;
	new_environ = malloc((count + 3) * sizeof(char *));
	if (! new_environ)
		return (NULL);
	k = 0;
	while (k < count + 3)
	{
		new_environ[k] = NULL;
		k++;
	}
	return (new_environ);
}

char	*create_new_pwd_var(char *new_pwd)
{
	char	*new_pwd_var;

	new_pwd_var = malloc(ft_strlen("PWD") + ft_strlen(new_pwd) + 2);
	if (! new_pwd_var)
		return (NULL);
	ft_strcpy(new_pwd_var, "PWD=");
	ft_strlcat(new_pwd_var, new_pwd,
		ft_strlen("PWD") + ft_strlen(new_pwd) + 2);
	return (new_pwd_var);
}

void	copy_and_update_env_vars(char ***env_copy, char **new_environ,
		char *old_pwd_var, char *new_pwd_var)
{
	int					flags[2];
	t_env_update_params	params;

	flags[0] = 0;
	flags[1] = 0;
	params.env_copy = env_copy;
	params.new_environ = new_environ;
	params.old_pwd_var = old_pwd_var;
	params.new_pwd_var = new_pwd_var;
	params.flags = flags;
	copy_existing_env_vars(params);
	add_missing_env_vars(params);
}
