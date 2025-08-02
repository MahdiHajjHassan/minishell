/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_numeric_arg(char *arg)
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

int	check_too_many_args(char **argv)
{
	if (argv[2])
	{
		ft_fprintf_stderr("minishell: exit: too many arguments\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	process_exit_arg(char **argv)
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
