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

int	update_pwd_and_cleanup(char *old_pwd, char ***env_copy,
	const char *arg_for_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd_dup_quiet();
	if (!new_pwd)
		new_pwd = get_fallback_pwd(env_copy, arg_for_pwd);
	if (!new_pwd)
	{
		free(old_pwd);
		return (1);
	}
	if (old_pwd)
		update_pwd_variables(old_pwd, new_pwd, env_copy);
	else
		update_pwd_variables("", new_pwd, env_copy);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(char **argv, char ***env_copy)
{
	char	*old_pwd;

	old_pwd = setup_old_pwd(env_copy);
	if (!old_pwd)
		return (1);
	if (!argv[1])
	{
		if (handle_cd_no_args(old_pwd, env_copy))
			return (1);
	}
	else
	{
		if (handle_cd_with_args(argv, old_pwd, env_copy))
			return (1);
	}
	return (update_pwd_and_cleanup(old_pwd, env_copy,
			argv[1]));
}
