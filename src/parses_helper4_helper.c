/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_single_argument_internal(t_arg_process_params arg_params)
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
	return (processed);
}

int	concatenate_adjacent_tokens(t_arg_process_params arg_params)
{
	char	*result;
	char	*next_q;
	char	*next_eq;

	result = process_single_argument_internal(arg_params);
	if (!result)
		return (1);
	while (1)
	{
		if (!should_concatenate_next_token(arg_params))
			break ;
		if (process_next_token_for_concatenation(arg_params, &result,
				&next_q, &next_eq))
			return (1);
		*arg_params.eq = next_eq;
	}
	add_argument(arg_params.params.cmd, result, arg_params.params.argc);
	return (0);
}

static int	handle_argument_token(t_arg_process_params arg_params)
{
	int							consecutive_quotes;
	t_consecutive_quotes_params	quote_params;

	consecutive_quotes = count_consecutive_quotes(arg_params.q, arg_params.eq);
	if ((**arg_params.q == '"' && *(*arg_params.eq - 1) == '"')
		|| (**arg_params.q == '\'' && *(*arg_params.eq - 1) == '\''))
		return (handle_quoted_argument_token(arg_params));
	if (consecutive_quotes > 1 && (**arg_params.q == '"'
			|| **arg_params.q == '\''))
	{
		quote_params.ret = arg_params.ret;
		quote_params.params = arg_params.params;
		quote_params.env_copy = arg_params.env_copy;
		quote_params.q = arg_params.q;
		quote_params.eq = arg_params.eq;
		return (process_consecutive_quotes(quote_params));
	}
	else
		return (handle_unquoted_argument_token(arg_params));
}

static int	handle_argument_token_type(t_token_type_params token_params)
{
	t_arg_process_params	arg_params;

	arg_params.ret = token_params.ret;
	arg_params.params = token_params.params;
	arg_params.env_copy = token_params.env_copy;
	arg_params.q = token_params.q;
	arg_params.eq = token_params.eq;
	return (handle_argument_token(arg_params));
}

int	handle_token_type(t_token_type_params token_params)
{
	if (token_params.tok == '<' || token_params.tok == '>'
		|| token_params.tok == '+' || token_params.tok == 'H')
		return (handle_redirection_token_type(token_params));
	else if (token_params.tok == 'a' || token_params.tok == '"'
		|| token_params.tok == '\'')
		return (handle_argument_token_type(token_params));
	else
		return (1);
}
