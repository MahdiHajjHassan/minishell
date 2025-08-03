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

static int	process_single_argument(t_arg_process_params arg_params)
{
	char	*processed;
	int		quote_type;
	char	*q_copy;
	char	*eq_copy;

	q_copy = *arg_params.q;
	eq_copy = *arg_params.eq;
	
	// DEBUG: Print the argument before processing
	// printf("DEBUG process_single_argument: input='%.*s'\n", (int)(eq_copy - q_copy), q_copy);
	
	quote_type = remove_exec_quotes(&q_copy, &eq_copy);
	
	// DEBUG: Print after quote removal
	// printf("DEBUG after remove_exec_quotes: quote_type=%c, arg='%.*s'\n", 
	//        quote_type ? quote_type : '0', (int)(eq_copy - q_copy), q_copy);
	
	processed = process_argument_with_expansion(q_copy,
			eq_copy, arg_params.env_copy, quote_type);
			
	// DEBUG: Print the processed result
	// printf("DEBUG after process_argument_with_expansion: processed='%s'\n", processed ? processed : "NULL");
	
	if (! processed)
	{
		free_cmd(*arg_params.ret);
		return (1);
	}
	add_argument(arg_params.params.cmd, processed, arg_params.params.argc);
	return (0);
}

static int	handle_argument_token(t_arg_process_params arg_params)
{
	int							consecutive_quotes;
	t_consecutive_quotes_params	quote_params;

	// DEBUG: Print what we're handling
	// printf("DEBUG handle_argument_token: arg='%.*s'\n", 
	//        (int)(*arg_params.eq - *arg_params.q), *arg_params.q);
	
	consecutive_quotes = count_consecutive_quotes(arg_params.q, arg_params.eq);
	
	// DEBUG: Print quote count
	// printf("DEBUG consecutive_quotes: %d\n", consecutive_quotes);
	
	if ((**arg_params.q == '"' && *(*arg_params.eq - 1) == '"')
		|| (**arg_params.q == '\'' && *(*arg_params.eq - 1) == '\''))
	{
		// DEBUG
		// printf("DEBUG: Entire argument is quoted, using process_single_argument\n");
		return (process_single_argument(arg_params));
	}
	if (consecutive_quotes > 1)
	{
		// DEBUG
		// printf("DEBUG: Multiple quotes found, using process_consecutive_quotes\n");
		quote_params.ret = arg_params.ret;
		quote_params.params = arg_params.params;
		quote_params.env_copy = arg_params.env_copy;
		quote_params.q = arg_params.q;
		quote_params.eq = arg_params.eq;
		return (process_consecutive_quotes(quote_params));
	}
	else
	{
		// DEBUG
		// printf("DEBUG: Default case, using process_single_argument\n");
		return (process_single_argument(arg_params));
	}
}

static int	handle_redirection_token_type(t_token_type_params token_params)
{
	t_redir_token_params	redir_params;

	redir_params.ret = token_params.ret;
	redir_params.params = token_params.params;
	redir_params.env_copy = token_params.env_copy;
	redir_params.tok = token_params.tok;
	redir_params.q = token_params.q;
	redir_params.eq = token_params.eq;
	return (handle_redirection_token(redir_params));
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
