/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper4.c                                :+:      :+:    :+:  */
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
		print_cd_getcwd_failed();
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

int	handle_cd_with_args(char **argv, char *old_pwd, char ***env_copy)
{
	if (argv[2])
	{
		print_cd_too_many_args();
		free(old_pwd);
		return (1);
	}
	if (ft_strcmp(argv[1], "-") == 0)
		return (handle_cd_dash(old_pwd, env_copy));
	return (handle_cd_regular_path(argv, old_pwd));
}

char	*compute_logical_pwd(const char *old_pwd, const char *path)
{
	char	*result;
	size_t	old_len;
	size_t	path_len;

	if (!old_pwd || !path)
		return (NULL);
	if (ft_strcmp(path, "-") == 0)
		return (NULL);
	old_len = ft_strlen(old_pwd);
	path_len = ft_strlen(path);
	result = malloc(old_len + 1 + path_len + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, old_pwd);
	if (old_len == 0 || old_pwd[old_len - 1] != '/')
		ft_strlcat(result, "/", old_len + 1 + path_len + 1);
	ft_strlcat(result, path, old_len + 1 + path_len + 1);
	return (result);
}
