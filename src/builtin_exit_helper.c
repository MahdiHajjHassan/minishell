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

static int	parse_exit_ll(const char *arg, long long *out)
{
	long long	result;
	int			digit;
	int			neg;
	int			i;

	if (!arg || !*arg)
	{
		print_exit_numeric_arg(arg ? arg : "");
		return (0);
	}
	i = 0;
	neg = 0;
	result = 0;
	if (arg[i] == '+' || arg[i] == '-')
	{
		neg = (arg[i] == '-');
		i++;
	}
	if (!arg[i] || !ft_isdigit(arg[i]))
	{
		print_exit_numeric_arg(arg);
		return (0);
	}
	if (neg)
	{
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]))
			{
				print_exit_numeric_arg(arg);
				return (0);
			}
			digit = arg[i] - '0';
			if (result < (LLONG_MIN + digit) / 10)
			{
				print_exit_numeric_arg(arg);
				return (0);
			}
			result = result * 10 - digit;
			i++;
		}
	}
	else
	{
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]))
			{
				print_exit_numeric_arg(arg);
				return (0);
			}
			digit = arg[i] - '0';
			if (result > (LLONG_MAX - digit) / 10)
			{
				print_exit_numeric_arg(arg);
				return (0);
			}
			result = result * 10 + digit;
			i++;
		}
	}
	*out = result;
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
	long long	ll_value;
	int			value;

	if (! argv[1])
		return (0);
	if (! parse_exit_ll(argv[1], &ll_value))
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
