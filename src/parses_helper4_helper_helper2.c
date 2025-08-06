/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper_helper2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_single_argument(t_arg_process_params arg_params)
{
	char	*processed;
	int		quote_type;
	char	*q_copy;
	char	*eq_copy;

	q_copy = *arg_params.q;
	eq_copy = *arg_params.eq;
	quote_type = remove_exec_quotes(&q_copy, &eq_copy);
	processed = process_argument_with_expansion(q_copy,
			eq_copy, arg_params.env_copy, quote_type);
	if (!processed)
	{
		free_cmd(*arg_params.ret);
		return (1);
	}
	add_argument(arg_params.params.cmd, processed, arg_params.params.argc);
	return (0);
}

int	handle_quoted_argument_token(t_arg_process_params arg_params)
{
	char	*next_pos;

	next_pos = *arg_params.eq;
	while (next_pos < arg_params.params.input_end
		&& (*next_pos == ' ' || *next_pos == '\t'))
		next_pos++;
	if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end
		&& *next_pos != '"' && *next_pos != '\'' && *next_pos != '|'
		&& *next_pos != '&' && *next_pos != ';' && *next_pos != '<'
		&& *next_pos != '>')
		return (concatenate_adjacent_tokens(arg_params));
	return (process_single_argument(arg_params));
}

int	handle_unquoted_argument_token(t_arg_process_params arg_params)
{
	char	*next_pos;

	next_pos = *arg_params.eq;
	while (next_pos < arg_params.params.input_end
		&& (*next_pos == ' ' || *next_pos == '\t'))
		next_pos++;
	if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end
		&& *next_pos != '"' && *next_pos != '\'' && *next_pos != '|'
		&& *next_pos != '&' && *next_pos != ';' && *next_pos != '<'
		&& *next_pos != '>')
		return (concatenate_adjacent_tokens(arg_params));
	return (process_single_argument(arg_params));
}
