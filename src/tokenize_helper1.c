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

char	*skip_whitespace(char *s, char *input_end, char *space)
{
	while (s < input_end && ft_strchr(space, *s))
		s++;
	return (s);
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
	ret = handle_less_than(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	return ('a');
}

static char	*process_token_loop(char *s, char *input_end, t_token_params params)
{
	char	quote;

	quote = 0;
	while (s < input_end)
	{
		if (!quote)
		{
			if (ft_strchr(params.symbols, *s) || ft_strchr(params.space, *s))
				break ;
			if (*s == '"' || *s == '\'')
				quote = *s;
		}
		else if (quote && *s == quote)
		{
			quote = 0;
			s++;
			break ;
		}
		s++;
	}
	if (quote)
		return (NULL);
	return (s);
}

char	*handle_default_token(char *s, char *input_end, char *input_ptr,
		t_token_params params)
{
	(void)input_ptr;
	return (process_token_loop(s, input_end, params));
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
