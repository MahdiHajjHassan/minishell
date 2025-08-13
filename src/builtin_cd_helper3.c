/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getcwd_dup_quiet(void)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	return (ft_strdup(cwd));
}

char	*get_env_value_simple(const char *name, char **env_copy)
{
	size_t	name_len;
	int		i;

	if (!name || !env_copy)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env_copy[i])
	{
		if (ft_strncmp(env_copy[i], name, name_len) == 0
			&& env_copy[i][name_len] == '=')
			return (env_copy[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

char	*get_fallback_pwd(char ***env_copy, const char *arg_for_pwd)
{
	char	*pwd_val;
	char	*new_pwd;

	pwd_val = get_env_value_simple("PWD", *env_copy);
	if (pwd_val)
	{
		if (arg_for_pwd && arg_for_pwd[0] == '/')
			new_pwd = ft_strdup(arg_for_pwd);
		else if (arg_for_pwd)
			new_pwd = compute_logical_pwd(pwd_val, arg_for_pwd);
		if (!new_pwd)
			new_pwd = ft_strdup(pwd_val);
		return (new_pwd);
	}
	return (NULL);
}

char	*setup_old_pwd(char ***env_copy)
{
	char	*old_pwd;
	char	*pwd_env_value;

	old_pwd = getcwd_dup_quiet();
	if (!old_pwd)
	{
		print_cd_getcwd_failed();
		pwd_env_value = get_env_value_simple("PWD", *env_copy);
		if (pwd_env_value)
			old_pwd = ft_strdup(pwd_env_value);
		else
			old_pwd = NULL;
	}
	return (old_pwd);
}
