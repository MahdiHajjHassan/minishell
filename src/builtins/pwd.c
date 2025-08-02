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

static char	*make_pwd_var(const char *prefix, const char *value)
{
	char	*var;
	size_t	size;

	size = ft_strlen(prefix) + ft_strlen(value) + 1;
	var = malloc(size);
	if (!var)
		return (NULL);
	ft_strcpy(var, prefix);
	ft_strlcat(var, value, size);
	return (var);
}

static int	count_env(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

static void	assign_or_add_pwd_vars(char **new_env, int i, int count,
					char *old_pwd_var, char *new_pwd_var)
{
	if (i == count)
	{
		new_env[i] = old_pwd_var;
		new_env[i + 1] = new_pwd_var;
		new_env[i + 2] = NULL;
	}
	else
		new_env[i] = NULL;
}

static void	copy_env_with_new_pwd(char **src, char **dst, int count,
					char *old_pwd_var, char *new_pwd_var)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		if (ft_strncmp(src[i], "OLDPWD=", 7) == 0)
		{
			free(src[i]);
			dst[i] = old_pwd_var;
		}
		else if (ft_strncmp(src[i], "PWD=", 4) == 0)
		{
			free(src[i]);
			dst[i] = new_pwd_var;
		}
		else
			dst[i] = src[i];
		i++;
	}
	assign_or_add_pwd_vars(dst, i, count, old_pwd_var, new_pwd_var);
}

void	update_pwd_variables(char *old_pwd, char *new_pwd, char ***env_copy)
{
	char	*old_pwd_var;
	char	*new_pwd_var;
	int		count;
	char	**new_environ;

	old_pwd_var = make_pwd_var("OLDPWD=", old_pwd);
	new_pwd_var = make_pwd_var("PWD=", new_pwd);
	if (!old_pwd_var || !new_pwd_var)
	{
		free(old_pwd_var);
		free(new_pwd_var);
		return ;
	}
	count = count_env(*env_copy);
	new_environ = malloc((count + 3) * sizeof(char *));
	if (!new_environ)
	{
		free(old_pwd_var);
		free(new_pwd_var);
		return ;
	}
	copy_env_with_new_pwd(*env_copy, new_environ, count, old_pwd_var, new_pwd_var);
	free(*env_copy);
	*env_copy = new_environ;
}
