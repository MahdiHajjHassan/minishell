/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_escaped(const char *s, const char *start)
{
	int	count;

	count = 0;
	while (s > start && *(s - 1) == '\\')
	{
		count++;
		s--;
	}
	return (count % 2);
}

char	*skip_whitespace(char *s, char *input_end, char *space)
{
	while (s < input_end && strchr(space, *s))
		s++;
	return (s);
}

int	handle_basic_symbols(char **s_ptr, char *input_ptr)
{
	char	*s;
	int		ret;

	s = *s_ptr;
	ret = *s;
	if (*s == '|' || *s == '&' || *s == ';' || *s == '('
		|| *s == ')' || *s == '<')
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

int	handle_special_chars(char **s_ptr, char *input_ptr)
{
	int	ret;

	ret = handle_basic_symbols(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	ret = handle_greater_than(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	return ('a');
}

char	*handle_default_token(char *s, char *input_end, char *input_ptr,
								t_token_params params)
{
	char	quote;

	quote = 0;
	while (s < input_end)
	{
		if (!quote && !is_escaped(s, input_ptr))
		{
			if (strchr(params.symbols, *s) || strchr(params.space, *s))
				break ;
			if (*s == '"' || *s == '\'')
				quote = *s;
		}
		else if (quote && !is_escaped(s, input_ptr) && *s == quote)
		{
			quote = 0;
		}
		s++;
	}
	return (s);
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
	symbols[4] = ';';
	symbols[5] = '(';
	symbols[6] = ')';
	symbols[7] = '\0';
}

void	init_token_arrays(char *space, char *symbols)
{
	init_space_array(space);
	init_symbols_array(symbols);
}

t_token_params	setup_token_params(char *s, char *input_end,
								t_token_params params)
{
	params.s = s;
	params.input_end = input_end;
	return (params);
}

int	process_default_case(char **s_ptr, char *input_end,
								t_process_default_params params)
{
	t_token_params	token_params;

	token_params.input_ptr = params.input_ptr;
	token_params.symbols = params.symbols;
	token_params.space = params.space;
	token_params = setup_token_params(*s_ptr, input_end, token_params);
	*s_ptr = handle_default_token(*s_ptr, input_end, params.input_ptr,
			token_params);
	return ('a');
}

void	setup_token_processing(char **input_ptr, char *input_end,
								char **token_start, char **s_ptr)
{
	char	space[6];
	char	symbols[8];

	init_token_arrays(space, symbols);
	*s_ptr = *input_ptr;
	*s_ptr = skip_whitespace(*s_ptr, input_end, space);
	if (token_start)
		*token_start = *s_ptr;
}

int	handle_token_cases(char **s_ptr, char *input_ptr,
								char *input_end)
{
	int							ret;
	t_process_default_params	params;
	char						space[6];
	char						symbols[8];

	init_token_arrays(space, symbols);
	ret = handle_special_chars(s_ptr, input_ptr);
	if (ret == 'a')
	{
		params.s_ptr = s_ptr;
		params.input_end = input_end;
		params.input_ptr = input_ptr;
		params.symbols = symbols;
		params.space = space;
		ret = process_default_case(s_ptr, input_end, params);
	}
	return (ret);
} 