/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_break_on_char(char c, t_token_params params, int quote)
{
	if (!quote)
	{
		if (ft_strchr(params.symbols, c) || ft_strchr(params.space, c))
			return (1);
	}
	return (0);
}

char	*process_token_loop(char *s, char *input_end, t_token_params params)
{
	char	quote;
	char	*start_pos;
    char	*result;

	quote = 0;
	start_pos = s;
    while (s < input_end)
    {
        if (should_break_on_char(*s, params, quote))
            break ;
        if (!quote && (*s == '"' || *s == '\''))
        {
            if (s != start_pos)
                break ;
        }
        result = handle_quote_logic(s, &quote);
        if (result)
            return (result);
        s++;
        if (s - start_pos > 10000)
        {
            if (quote)
                return (NULL);
            break ;
        }
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
