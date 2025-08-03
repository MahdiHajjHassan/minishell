/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:46:59 by mahajj-h          #+#    #+#             */
/*   Updated: 2024/06/17 12:51:07 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

static int	skip_whitespace(const char *str, int *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	return (*i);
}

static int	handle_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static long	convert_digits(const char *str, int *i, int sign)
{
	long	result;

	result = 0;
	while (ft_isdigit(str[*i]))
	{
		result = result * 10 + (str[*i] - '0');
		if (sign == 1 && result > INT_MAX)
			return (INT_MAX);
		if (sign == -1 && result > (long)INT_MAX + 1)
			return (INT_MIN);
		(*i)++;
	}
	return (result);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	result;
	int		i;

	if (!str)
		return (0);
	i = 0;
	skip_whitespace(str, &i);
	sign = handle_sign(str, &i);
	result = convert_digits(str, &i, sign);
	return ((int)(result * sign));
}
