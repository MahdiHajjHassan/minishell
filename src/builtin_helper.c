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

int	process_export_arg(char *arg_copy, char **name, char **value,
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

int	is_valid_identifier(const char *name)
{
	size_t	i;

	if (!name || !*name)
		return (0);
	if ((*name < 'A' || *name > 'Z') && (*name < 'a' || *name > 'z')
		&& *name != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if ((name[i] < 'A' || name[i] > 'Z')
			&& (name[i] < 'a' || name[i] > 'z')
			&& (name[i] < '0' || name[i] > '9')
			&& name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	process_single_export_arg(char **argv, int i, char ***env_copy)
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
		if (!is_valid_identifier(arg_copy))
		{
			print_export_invalid_identifier(arg_copy);
			free(arg_copy);
			return (1);
		}
		free(arg_copy);
		return (0);
	}
	if (process_export_arg(arg_copy, &name, &value, env_copy))
		return (1);
	return (0);
}

int	builtin_export(char **argv, char ***env_copy)
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

int	builtin_unset(char **argv, char ***env_copy)
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
