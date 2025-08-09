/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_arg_params(t_arg_process_params arg_params)
{
	if (!arg_params.q || !*arg_params.q || !arg_params.eq || !*arg_params.eq)
		return (1);
	if (*arg_params.eq <= *arg_params.q)
		return (1);
	if ((char *)*arg_params.q < (char *)arg_params.params.input_ptr
		|| (char *)*arg_params.eq > (char *)arg_params.params.input_end
		|| *arg_params.eq <= *arg_params.q)
		return (1);
	return (0);
}

int	handle_consecutive_quotes_case(t_arg_process_params arg_params)
{
	t_consecutive_quotes_params	quote_params;

	quote_params.ret = arg_params.ret;
	quote_params.params = arg_params.params;
	quote_params.env_copy = arg_params.env_copy;
	quote_params.q = arg_params.q;
	quote_params.eq = arg_params.eq;
	return (process_consecutive_quotes(quote_params));
}

int	handle_argument_token(t_arg_process_params arg_params)
{
	int	consecutive_quotes;

	if (validate_arg_params(arg_params))
		return (handle_unquoted_argument_token(arg_params));
	consecutive_quotes = count_consecutive_quotes(arg_params.q, arg_params.eq);
	if ((**arg_params.q == '"' && *(*arg_params.eq - 1) == '"')
		|| (**arg_params.q == '\'' && *(*arg_params.eq - 1) == '\''))
		return (handle_quoted_argument_token(arg_params));
	if (consecutive_quotes > 1 && (**arg_params.q == '"'
			|| **arg_params.q == '\''))
		return (handle_consecutive_quotes_case(arg_params));
	else
		return (handle_unquoted_argument_token(arg_params));
}

int	handle_argument_token_type(t_token_type_params token_params)
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
