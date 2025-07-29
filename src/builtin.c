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

static int	process_export_arg(char *arg_copy, char **name, char **value)
{
	if (parse_export_arg(arg_copy, name, value))
	{
		free(arg_copy);
		return (1);
	}
	remove_quotes(value);
	if (set_environment_var(*name, *value))
	{
		free(arg_copy);
		return (1);
	}
	free(arg_copy);
	return (0);
}

static int	builtin_export(char **argv)
{
	extern char	**environ;
	int			i;
	char		*name;
	char		*value;
	char		*arg_copy;
	char		*equals;

	/* If no arguments, print all environment variables */
	if (!argv[1])
	{
		i = 0;
		while (environ[i])
		{
			printf("declare -x %s\n", environ[i]);
			i++;
		}
		return (0);
	}

	i = 1;
	while (argv[i])
	{
		arg_copy = ft_strdup(argv[i]);
		if (!arg_copy)
			return (1);
		
		equals = ft_strchr(arg_copy, '=');
		if (!equals)
		{
			/* Variable without value - just check if it exists */
			free(arg_copy);
			ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n", argv[i]);
			return (1);
		}
		
		if (process_export_arg(arg_copy, &name, &value))
			return (1);
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_unsetenv(argv[i]) != 0)
		{
			perror("unset");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	builtin_env(char **argv)
{
	extern char	**environ;
	int			i;

	(void)argv;
	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	handle_builtin(char **argv)
{
	if (!argv[0])
		return (1);
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(argv));
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(argv));
	if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
