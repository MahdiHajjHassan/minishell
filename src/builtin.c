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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

int	is_builtin(char *cmd)
{
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	if (!strcmp(cmd, "export"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static int	builtin_export(char **argv)
{
	int		i;
	char	*name;
	char	*value;

	i = 1;
	while (argv[i])
	{
		if (parse_export_arg(argv[i], &name, &value))
			return (1);
		remove_quotes(&value);
		if (set_environment_var(name, value))
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
		if (unsetenv(argv[i]) != 0)
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
	if (!strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (!strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	if (!strcmp(argv[0], "pwd"))
		return (builtin_pwd(argv));
	if (!strcmp(argv[0], "export"))
		return (builtin_export(argv));
	if (!strcmp(argv[0], "unset"))
		return (builtin_unset(argv));
	if (!strcmp(argv[0], "env"))
		return (builtin_env(argv));
	if (!strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
