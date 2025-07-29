/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_basic_symbols(char **s_ptr, char *input_ptr)
{
	char	*s;
	int		ret;

	s = *s_ptr;
	ret = *s;
	if (*s == '|' || *s == '&' || *s == '('
		|| *s == ')')
	{
		if (!is_escaped(s, input_ptr))
		{
			s++;
			*s_ptr = s;
			return (ret);
		}
		*s_ptr = s;
		return ('a');
	}
	*s_ptr = s;
	return (-1);
}

int	handle_greater_than(char **s_ptr, char *input_ptr)
{
	char	*s;
	int		ret;

	s = *s_ptr;
	ret = *s;
	if (*s == '>')
	{
		if (!is_escaped(s, input_ptr))
		{
			s++;
			if (*s == '>' && !is_escaped(s, input_ptr))
			{
				ret = '+';
				s++;
			}
			*s_ptr = s;
			return (ret);
		}
		*s_ptr = s;
		return ('a');
	}
	*s_ptr = s;
	return (-1);
}

void	init_space_array(char *space)
{
	space[0] = ' ';
	space[1] = '\t';
	space[2] = '\r';
	space[3] = '\n';
	space[4] = '\v';
	space[5] = '\0';
}

void	init_symbols_array(char *symbols)
{
	symbols[0] = '<';
	symbols[1] = '>';
	symbols[2] = '|';
	symbols[3] = '&';
	/* Semicolon removed - not supported in this minishell */
	symbols[4] = '(';
	symbols[5] = ')';
	symbols[6] = '\0';
}

int	handle_less_than(char **s_ptr, char *input_ptr)
{
	char	*s;
	int		ret;

	s = *s_ptr;
	ret = *s;
	if (*s == '<')
	{
		if (!is_escaped(s, input_ptr))
		{
			s++;
			if (*s == '<' && !is_escaped(s, input_ptr))
			{
				ret = 'H';
				s++;
			}
			*s_ptr = s;
			return (ret);
		}
		*s_ptr = s;
		return ('a');
	}
	*s_ptr = s;
	return (-1);
}

void	init_token_arrays(char *space, char *symbols)
{
	init_space_array(space);
	init_symbols_array(symbols);
}
