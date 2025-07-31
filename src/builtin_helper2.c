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
	while (argv[i] && !ft_strcmp(argv[i], "-n"))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int	builtin_cd(char **argv, char ***env_copy)
{
	char	*old_pwd;
	char	*new_pwd;
	char	cwd[1024];

	/* Get current directory before changing */
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf_stderr("minishell: cd: getcwd failed\n");
		return (1);
	}
	old_pwd = ft_strdup(cwd);

	/* Change directory */
	if (!argv[1])
	{
		if (cd_to_home(*env_copy) != 0)
		{
			free(old_pwd);
			return (1);
		}
	}
	else if (argv[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		free(old_pwd);
		return (1);
	}
	else
	{
		if (cd_to_path(argv[1]) != 0)
		{
			free(old_pwd);
			return (1);
		}
	}

	/* Get new directory after changing */
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf_stderr("minishell: cd: getcwd failed\n");
		free(old_pwd);
		return (1);
	}
	new_pwd = ft_strdup(cwd);

	/* Update PWD and OLDPWD in environment */
	update_pwd_variables(old_pwd, new_pwd, env_copy);

	free(old_pwd);
	free(new_pwd);
	return (0);
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
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
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
		
		if (argv[2])
		{
			ft_fprintf_stderr("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1);
		}
	}
	
	clean_exit(status);
	return (0);
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
