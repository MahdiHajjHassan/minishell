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
	if (argv[1] && !ft_strcmp(argv[1], "-n"))
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
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
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
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	
	if (argv[1])
	{
		i = 0;
		if (argv[1][i] == '-' || argv[1][i] == '+')
			i++;
		while (argv[1][i])
		{
			if (!ft_isdigit(argv[1][i]))
			{
				ft_fprintf_stderr("minishell: exit: ", STDERR_FILENO);
				ft_fprintf_stderr(argv[1], STDERR_FILENO);
				ft_fprintf_stderr(": numeric argument required\n", STDERR_FILENO);
				clean_exit(255);
			}
			i++;
		}
		status = ft_atoi(argv[1]);
		
		/* Check for too many arguments */
		if (argv[2])
		{
			ft_fprintf_stderr("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1);  /* Don't exit, just return error */
		}
	}
	
	clean_exit(status);
	return (0);  /* Never reached */
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}
