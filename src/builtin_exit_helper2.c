/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helper2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_negative_number(const char *arg, long long *result, int i)
{
	int	digit;

	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			print_exit_numeric_arg(arg);
			return (0);
		}
		digit = arg[i] - '0';
		if (*result < (LLONG_MIN + digit) / 10)
		{
			print_exit_numeric_arg(arg);
			return (0);
		}
		*result = *result * 10 - digit;
		i++;
	}
	return (1);
}

static int	parse_positive_number(const char *arg, long long *result, int i)
{
	int	digit;

	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			print_exit_numeric_arg(arg);
			return (0);
		}
		digit = arg[i] - '0';
		if (*result > (LLONG_MAX - digit) / 10)
		{
			print_exit_numeric_arg(arg);
			return (0);
		}
		*result = *result * 10 + digit;
		i++;
	}
	return (1);
}

static int	validate_exit_arg(const char *arg, int *i, int *neg)
{
	if (!arg || !*arg)
	{
		if (arg)
			print_exit_numeric_arg(arg);
		else
			print_exit_numeric_arg("");
		return (0);
	}
	*i = 0;
	*neg = 0;
	if (arg[*i] == '+' || arg[*i] == '-')
	{
		*neg = (arg[*i] == '-');
		(*i)++;
	}
	if (!arg[*i] || !ft_isdigit(arg[*i]))
	{
		print_exit_numeric_arg(arg);
		return (0);
	}
	return (1);
}

int	parse_exit_ll(const char *arg, long long *out)
{
	long long	result;
	int			neg;
	int			i;

	if (!validate_exit_arg(arg, &i, &neg))
		return (0);
	result = 0;
	if (neg)
	{
		if (!parse_negative_number(arg, &result, i))
			return (0);
	}
	else
	{
		if (!parse_positive_number(arg, &result, i))
			return (0);
	}
	*out = result;
	return (1);
}
