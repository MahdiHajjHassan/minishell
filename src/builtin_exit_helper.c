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
	long long	ll_value;
	int			value;

	if (!argv[1])
		return (0);
	if (!parse_exit_ll(argv[1], &ll_value))
		return (2);
	if (check_too_many_args(argv))
		return (1);
	value = (int)((unsigned char)ll_value);
	return (value);
}

int	builtin_exit(char **argv)
{
	int	status;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	status = process_exit_arg(argv);
	return (status);
}
