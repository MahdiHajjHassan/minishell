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
			print_exit_numeric_arg(arg);
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
		print_exit_too_many_args();
		return (1);
	}
	return (0);
}

int	process_exit_arg(char **argv)
{
	if (! argv[1])
		return (0);
	if (! validate_numeric_arg(argv[1]))
		clean_exit(2);
	if (check_too_many_args(argv))
		return (1);
	return (atoi(argv[1]));
}

int	builtin_exit(char **argv)
{
	int	status;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	status = process_exit_arg(argv);
	clean_exit(status);
	return (0);
}
