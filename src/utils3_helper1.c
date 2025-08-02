/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdarg.h>

int	handle_d_format(char *str, int *i, int size, va_list args)
{
	int		num;
	char	*num_str;
	int		j;

	num = va_arg(args, int);
	num_str = ft_itoa(num);
	if (! num_str)
		return (-1);
	j = 0;
	while (num_str[j] && *i < size - 1)
	{
		str[*i] = num_str[j];
		(*i)++;
		j++;
	}
	free(num_str);
	return (0);
}

int	handle_s_format(char *str, int *i, int size, va_list args)
{
	char	*s;
	int		j;

	s = va_arg(args, char *);
	j = 0;
	while (s && s[j] && *i < size - 1)
	{
		str[*i] = s[j];
		(*i)++;
		j++;
	}
	return (0);
}

int	handle_c_format(char *str, int *i, int size, va_list args)
{
	char	c;

	c = va_arg(args, int);
	if (*i < size - 1)
	{
		str[*i] = c;
		(*i)++;
	}
	return (0);
}

int	handle_format_specifier(t_format_params params)
{
	if (**params.format == '%' && *(*params.format + 1) == 'd')
	{
		if (handle_d_format(params.str, params.i, params.size,
				params.args) == -1)
			return (-1);
		*params.format += 2;
	}
	else if (**params.format == '%' && *(*params.format + 1) == 's')
	{
		handle_s_format(params.str, params.i, params.size, params.args);
		*params.format += 2;
	}
	else if (**params.format == '%' && *(*params.format + 1) == 'c')
	{
		handle_c_format(params.str, params.i, params.size, params.args);
		*params.format += 2;
	}
	else
	{
		params.str[*params.i] = **params.format;
		(*params.i)++;
		(*params.format)++;
	}
	return (0);
}

int	process_format_loop(t_format_params params)
{
	while (**params.format && *params.i < params.size - 1)
	{
		if (handle_format_specifier(params) == -1)
			return (-1);
	}
	return (0);
}
