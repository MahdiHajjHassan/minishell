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

static int	check_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	j = 2;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

static int	process_n_flags(char **argv, int *i)
{
	int	print_newline;

	print_newline = 1;
	while (argv[*i] && check_n_flag(argv[*i]))
	{
		print_newline = 0;
		(*i)++;
	}
	return (print_newline);
}

static void	print_arguments(char **argv, int start_index)
{
	int	i;

	i = start_index;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = process_n_flags(argv, &i);
	print_arguments(argv, i);
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

static char	*get_current_directory(void)
{
	char	cwd[1024];

	if (! getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf_stderr("minishell: cd: getcwd failed\n");
		return (NULL);
	}
	return (ft_strdup(cwd));
}

static int	handle_cd_no_args(char *old_pwd, char ***env_copy)
{
	if (cd_to_home(*env_copy) != 0)
	{
		free(old_pwd);
		return (1);
	}
	return (0);
}

static int	handle_cd_with_args(char **argv, char *old_pwd)
{
	if (argv[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		free(old_pwd);
		return (1);
	}
	if (cd_to_path(argv[1]) != 0)
	{
		free(old_pwd);
		return (1);
	}
	return (0);
}

static int	update_pwd_and_cleanup(char *old_pwd, char ***env_copy)
{
	char	*new_pwd;

	new_pwd = get_current_directory();
	if (! new_pwd)
	{
		free(old_pwd);
		return (1);
	}
	update_pwd_variables(old_pwd, new_pwd, env_copy);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(char **argv, char ***env_copy)
{
	char	*old_pwd;

	old_pwd = get_current_directory();
	if (! old_pwd)
		return (1);
	if (! argv[1])
	{
		if (handle_cd_no_args(old_pwd, env_copy))
			return (1);
	}
	else
	{
		if (handle_cd_with_args(argv, old_pwd))
			return (1);
	}
	return (update_pwd_and_cleanup(old_pwd, env_copy));
}

int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (! getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

static int	validate_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (! ft_isdigit(arg[i]))
		{
			ft_fprintf_stderr("minishell: exit: ", STDERR_FILENO);
			ft_fprintf_stderr(arg, STDERR_FILENO);
			ft_fprintf_stderr(": numeric argument required\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	check_too_many_args(char **argv)
{
	if (argv[2])
	{
		ft_fprintf_stderr("minishell: exit: too many arguments\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	process_exit_arg(char **argv)
{
	if (! argv[1])
		return (0);
	if (! validate_numeric_arg(argv[1]))
		clean_exit(255);
	if (check_too_many_args(argv))
		return (1);
	return (ft_atoi(argv[1]));
}

int	builtin_exit(char **argv)
{
	int	status;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	status = process_exit_arg(argv);
	clean_exit(status);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (! ft_strcmp(cmd, "echo"))
		return (1);
	if (! ft_strcmp(cmd, "cd"))
		return (1);
	if (! ft_strcmp(cmd, "pwd"))
		return (1);
	if (! ft_strcmp(cmd, "export"))
		return (1);
	if (! ft_strcmp(cmd, "unset"))
		return (1);
	if (! ft_strcmp(cmd, "env"))
		return (1);
	if (! ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}
