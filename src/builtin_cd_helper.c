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
		print_cd_getcwd_failed();
		return (NULL);
	}
	return (ft_strdup(cwd));
}

static char	*getcwd_dup_quiet(void)
{
    char    cwd[1024];

    if (!getcwd(cwd, sizeof(cwd)))
        return (NULL);
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

static char	*get_env_value_simple(const char *name, char **env_copy)
{
    size_t name_len;
    int i;

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

char	*compute_logical_pwd(const char *old_pwd, const char *path)
{
    char    *result;
    size_t  old_len;
    size_t  path_len;

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

int	update_pwd_and_cleanup(char *old_pwd, char ***env_copy, const char *arg_for_pwd)
{
	char	*new_pwd;

    new_pwd = getcwd_dup_quiet();
    if (! new_pwd)
    {
        char *pwd_val = get_env_value_simple("PWD", *env_copy);
        if (pwd_val)
        {
            if (arg_for_pwd && arg_for_pwd[0] == '/')
                new_pwd = ft_strdup(arg_for_pwd);
            else if (arg_for_pwd)
                new_pwd = compute_logical_pwd(pwd_val, arg_for_pwd);
            if (!new_pwd)
                new_pwd = ft_strdup(pwd_val);
        }
    }
    if (! new_pwd)
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

    old_pwd = getcwd_dup_quiet();
    if (! old_pwd)
    {
        print_cd_getcwd_failed();
        {
            char *pwd_env_value = get_env_value_simple("PWD", *env_copy);
            if (pwd_env_value)
                old_pwd = ft_strdup(pwd_env_value);
            else
                old_pwd = NULL;
        }
    }
    if (! old_pwd)
        return (1);
    if (! argv[1])
    {
        if (handle_cd_no_args(old_pwd, env_copy))
            return (1);
    }
    else
    {
        if (handle_cd_with_args(argv, old_pwd, env_copy))
            return (1);
    }
    return (update_pwd_and_cleanup(old_pwd, env_copy, argv[1]));
}
