/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_export_arg(char *arg_copy, char **name, char **value,
		char ***env_copy)
{
	if (parse_export_arg(arg_copy, name, value))
	{
		free(arg_copy);
		return (1);
	}
	remove_quotes(value);
	if (set_environment_var(*name, *value, env_copy))
	{
		free(arg_copy);
		return (1);
	}
	free(arg_copy);
	return (0);
}

static int	process_single_export_arg(char **argv, int i, char ***env_copy)
{
	char	*arg_copy;
	char	*equals;
	char	*name;
	char	*value;

	arg_copy = ft_strdup(argv[i]);
	if (! arg_copy)
		return (1);
	equals = ft_strchr(arg_copy, '=');
	if (! equals)
	{
		free(arg_copy);
		return (0);
	}
	if (process_export_arg(arg_copy, &name, &value, env_copy))
		return (1);
	return (0);
}

static int	builtin_export(char **argv, char ***env_copy)
{
	int	i;

	if (! argv[1])
	{
		print_sorted_env_vars(*env_copy);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (process_single_export_arg(argv, i, env_copy))
			return (1);
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv, char ***env_copy)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_unsetenv(argv[i], env_copy) != 0)
		{
			perror("unset");
			return (1);
		}
		i++;
	}
	return (0);
}

int	handle_builtin(char **argv, char ***env_copy)
{
	if (! argv[0])
		return (1);
	if (! ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (! ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv, env_copy));
	if (! ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(argv));
	if (! ft_strcmp(argv[0], "export"))
		return (builtin_export(argv, env_copy));
	if (! ft_strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env_copy));
	if (! ft_strcmp(argv[0], "env"))
		return (builtin_env(argv, env_copy));
	if (! ft_strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
