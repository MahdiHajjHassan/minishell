/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char			*new_s_ptr;

	token_params.input_ptr = params.input_ptr;
	token_params.symbols = params.symbols;
	token_params.space = params.space;
	token_params = setup_token_params(*s_ptr, input_end, token_params);
	new_s_ptr = handle_default_token(*s_ptr, input_end, params.input_ptr,
			token_params);
	if (!new_s_ptr)
		return (0); /* Return 0 to indicate error */
	*s_ptr = new_s_ptr;
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

void	init_token_arrays(char *space, char *symbols)
{
	init_space_array(space);
	init_symbols_array(symbols);
}
