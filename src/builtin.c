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
	return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
		!strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
		!strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
		!strcmp(cmd, "exit"));
}

static int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	if (argv[1] && !strcmp(argv[1], "-n"))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
	return (0);
}

static int	builtin_cd(char **argv)
{
	char	*home;

	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) != 0)
		{
			fprintf(stderr, "minishell: cd: %s: %s\n", home, strerror(errno));
			return (1);
		}
	}
	else if (argv[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
	{
		if (chdir(argv[1]) != 0)
		{
			fprintf(stderr, "minishell: cd: %s: %s\n", argv[1], strerror(errno));
			return (1);
		}
	}
	return (0);
}

static int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	builtin_export(char **argv)
{
	int		i;
	char	*equals;
	char	*name;
	char	*value;

	i = 1;
	while (argv[i])
	{
		equals = strchr(argv[i], '=');
		if (!equals)
		{
			fprintf(stderr, "minishell: export: invalid format: %s\n", argv[i]);
			return (1);
		}
		name = argv[i];
		*equals = '\0';
		value = equals + 1;
		if (*value == '"' && value[strlen(value) - 1] == '"')
		{
			value++;
			value[strlen(value) - 1] = '\0';
		}
		else if (*value == '\'' && value[strlen(value) - 1] == '\'')
		{
			value++;
			value[strlen(value) - 1] = '\0';
		}
		if (setenv(name, value, 1) != 0)
		{
			fprintf(stderr, "minishell: export: %s\n", strerror(errno));
			return (1);
		}
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

static int	builtin_exit(char **argv)
{
	int	status;
	int	i;

	status = 0;
	if (argv[1])
	{
		i = 0;
		if (argv[1][i] == '-' || argv[1][i] == '+')
			i++;
		while (argv[1][i])
		{
			if (!isdigit(argv[1][i]))
			{
				fprintf(stderr, "minishell: exit: %s: numeric argument required\n", argv[1]);
				exit(255);
			}
			i++;
		}
		status = atoi(argv[1]);
	}
	printf("exit\n");
	exit(status);
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