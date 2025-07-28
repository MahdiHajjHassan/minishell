/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
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

int	builtin_cd(char **argv)
{
	if (!argv[1])
	{
		return (cd_to_home());
	}
	else if (argv[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
	{
		return (cd_to_path(argv[1]));
	}
}

int	builtin_pwd(char **argv)
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

int	builtin_exit(char **argv)
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
				fprintf(stderr,
					"minishell: exit: %s: numeric argument required\n",
					argv[1]);
				exit(255);
			}
			i++;
		}
		status = atoi(argv[1]);
	}
	printf("exit\n");
	exit(status);
} 