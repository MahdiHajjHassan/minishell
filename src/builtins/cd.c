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

int	cd_to_home(char **env_copy)
{
	char	*home;
	int		i;

	i = 0;
	while (env_copy && env_copy[i])
	{
		if (ft_strncmp(env_copy[i], "HOME=", 5) == 0)
		{
			home = env_copy[i] + 5;
			if (chdir(home) != 0)
			{
				ft_fprintf_stderr("minishell: cd: %s: %s\n", home,
					strerror(errno));
				return (1);
			}
			return (0);
		}
		i++;
	}
	ft_fprintf_stderr("minishell: cd: HOME not set\n");
	return (1);
}

int	cd_to_path(char *path)
{
	if (chdir(path) != 0)
	{
		ft_fprintf_stderr("minishell: cd: %s: %s\n",
			path, strerror(errno));
		return (1);
	}
	return (0);
}

int	builtin_cd(char **argv, char ***env_copy)
{
	char	cwd[1024];
	char	*old_pwd;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf_stderr("minishell: cd: getcwd failed\n");
		return (1);
	}
	old_pwd = ft_strdup(cwd);
	if (!old_pwd)
		return (1);
	return (change_directory(argv, env_copy, old_pwd));
}

static int	change_directory(char **argv, char ***env_copy, char *old_pwd)
{
	int	status;

	status = handle_cd_args(argv, env_copy, old_pwd);
	free(old_pwd);
	return (status);
}

static int	handle_cd_args(char **argv, char ***env_copy, char *old_pwd)
{
	if (!argv[1])
	{
		if (cd_to_home(*env_copy) != 0)
			return (1);
	}
	else if (argv[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		return (1);
	}
	else
	{
		if (cd_to_path(argv[1]) != 0)
			return (1);
	}
	return (update_pwd_and_return(old_pwd, env_copy));
}
