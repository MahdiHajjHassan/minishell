/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_directory(void)
{
	char	cwd[1024];

	if (! getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf_stderr("minishell: cd: getcwd failed\n");
		return (NULL);
	}
	return (ft_strdup(cwd));
}

int	handle_cd_no_args(char *old_pwd, char ***env_copy)
{
	if (cd_to_home(*env_copy) != 0)
	{
		free(old_pwd);
		return (1);
	}
	return (0);
}

int	handle_cd_with_args(char **argv, char *old_pwd)
{
	if (argv[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		free(old_pwd);
		return (1);
	}
	if (cd_to_path(argv[1]) != 0)
	{
		free(old_pwd);
		return (1);
	}
	return (0);
}

int	update_pwd_and_cleanup(char *old_pwd, char ***env_copy)
{
	char	*new_pwd;

	new_pwd = get_current_directory();
	if (! new_pwd)
	{
		free(old_pwd);
		return (1);
	}
	update_pwd_variables(old_pwd, new_pwd, env_copy);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(char **argv, char ***env_copy)
{
	char	*old_pwd;

	old_pwd = get_current_directory();
	if (! old_pwd)
		return (1);
	if (! argv[1])
	{
		if (handle_cd_no_args(old_pwd, env_copy))
			return (1);
	}
	else
	{
		if (handle_cd_with_args(argv, old_pwd))
			return (1);
	}
	return (update_pwd_and_cleanup(old_pwd, env_copy));
}
