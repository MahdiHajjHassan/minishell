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

static char	*process_single_argument_internal(t_arg_process_params arg_params)
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

static int	concatenate_adjacent_tokens(t_arg_process_params arg_params)
{
	char	*result;
	char	*next_q, *next_eq;
	int		next_tok;
	char	*temp;

	// Start with the current token
	result = process_single_argument_internal(arg_params);
	if (!result)
		return (1);

	// Keep looking for adjacent tokens to concatenate
	while (1)
	{
		char *next_pos = *arg_params.eq;
		// Skip whitespace
		while (next_pos < arg_params.params.input_end && 
			   (*next_pos == ' ' || *next_pos == '\t'))
			next_pos++;
		// Only concatenate if there's no whitespace
		if (next_pos != *arg_params.eq || next_pos >= arg_params.params.input_end || 
			*next_pos == '"' || *next_pos == '\'' || 
			*next_pos == '|' || *next_pos == '&' || *next_pos == ';' ||
			*next_pos == '<' || *next_pos == '>')
			break;

		// Get the next token
		next_tok = gettoken(arg_params.params.input_ptr, 
						   arg_params.params.input_end, &next_q, &next_eq);
		if (next_tok != 'a' && next_tok != '"' && next_tok != '\'')
			break;

		// Process the next token
		t_arg_process_params next_params = arg_params;
		next_params.q = &next_q;
		next_params.eq = &next_eq;
		temp = process_single_argument_internal(next_params);
		if (!temp)
		{
			free(result);
			return (1);
		}

		// Concatenate with the result
		char *new_result = malloc(ft_strlen(result) + ft_strlen(temp) + 1);
		if (!new_result)
		{
			free(result);
			free(temp);
			return (1);
		}
		ft_strcpy(new_result, result);
		ft_strlcat(new_result, temp, ft_strlen(result) + ft_strlen(temp) + 1);
		free(result);
		free(temp);
		result = new_result;

		// Update the current position for the next iteration
		*arg_params.eq = next_eq;
	}

	// Add the concatenated result
	add_argument(arg_params.params.cmd, result, arg_params.params.argc);
	return (0);
}

static int	process_single_argument(t_arg_process_params arg_params)
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

	consecutive_quotes = count_consecutive_quotes(arg_params.q, arg_params.eq);
	if ((**arg_params.q == '"' && *(*arg_params.eq - 1) == '"')
		|| (**arg_params.q == '\'' && *(*arg_params.eq - 1) == '\''))
	{
		// Check if this quoted token should be concatenated with the next token
		char *next_pos = *arg_params.eq;
		// Skip whitespace
		while (next_pos < arg_params.params.input_end && 
			   (*next_pos == ' ' || *next_pos == '\t'))
			next_pos++;
		// Only concatenate if there's no whitespace between the quote and the next token
		if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end && 
			*next_pos != '"' && *next_pos != '\'' && 
			*next_pos != '|' && *next_pos != '&' && *next_pos != ';' &&
			*next_pos != '<' && *next_pos != '>')
		{
			// Use the general concatenation function
			return (concatenate_adjacent_tokens(arg_params));
		}
		return (process_single_argument(arg_params));
	}
	if (consecutive_quotes > 1 && (**arg_params.q == '"'
			||**arg_params.q == '\''))
	{
		quote_params.ret = arg_params.ret;
		quote_params.params = arg_params.params;
		quote_params.env_copy = arg_params.env_copy;
		quote_params.q = arg_params.q;
		quote_params.eq = arg_params.eq;
		return (process_consecutive_quotes(quote_params));
	}
	else
	{
		// Check if this unquoted token should be concatenated with the next token
		char *next_pos = *arg_params.eq;
		// Skip whitespace
		while (next_pos < arg_params.params.input_end && 
			   (*next_pos == ' ' || *next_pos == '\t'))
			next_pos++;
		// Only concatenate if there's no whitespace between the token and the next token
		if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end && 
			*next_pos != '"' && *next_pos != '\'' && 
			*next_pos != '|' && *next_pos != '&' && *next_pos != ';' &&
			*next_pos != '<' && *next_pos != '>')
		{
			// Use the general concatenation function
			return (concatenate_adjacent_tokens(arg_params));
		}
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
