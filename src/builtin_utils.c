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

	(void)env_copy;
	new_environ = malloc((count + 3) * sizeof(char *));
	if (! new_environ)
		return (NULL);
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
	int		i;

	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], "OLDPWD=", 7) == 0)
		{
			new_environ[i] = old_pwd_var;
		}
		else if (ft_strncmp((*env_copy)[i], "PWD=", 4) == 0)
		{
			new_environ[i] = new_pwd_var;
		}
		else
		{
			new_environ[i] = (*env_copy)[i];
		}
		i++;
	}
}
