/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_oldpwd_value(char ***env_copy)
{
	char	*oldpwd_value;
	int		i;

	i = 0;
	oldpwd_value = NULL;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], "OLDPWD=", 7) == 0)
		{
			oldpwd_value = (*env_copy)[i] + 7;
			break ;
		}
		i++;
	}
	return (oldpwd_value);
}

int	handle_cd_dash(char *old_pwd, char ***env_copy)
{
	char	*oldpwd_value;

	oldpwd_value = find_oldpwd_value(env_copy);
	if (!oldpwd_value || ft_strlen(oldpwd_value) == 0)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	ft_putstr_fd(oldpwd_value, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (cd_to_path(oldpwd_value) != 0)
	{
		free(old_pwd);
		return (1);
	}
	return (0);
}

int	handle_cd_regular_path(char **argv, char *old_pwd)
{
    if (cd_to_path(argv[1]) != 0)
    {
        free(old_pwd);
        return (1);
    }
    return (0);
}
