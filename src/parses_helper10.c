/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper10.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection_token(t_redir_token_params params)
{
	int					filename_tok;
	t_redir_file_params	file_params;

	filename_tok = gettoken(params.params.input_ptr,
			params.params.input_end, params.q, params.eq);
	if (filename_tok != 'a')
		return (validate_filename_token(filename_tok, params.ret,
				params.params));
	file_params.ret = params.ret;
	file_params.params = params.params;
	file_params.env_copy = params.env_copy;
	file_params.tok = params.tok;
	file_params.q = params.q;
	file_params.eq = params.eq;
	return (process_redirection_file(file_params));
}

void	skip_to_quote_end(char **temp_ptr, char *temp_end, char quote_char)
{
	(*temp_ptr)++;
	while (*temp_ptr < temp_end && **temp_ptr != quote_char)
		(*temp_ptr)++;
}

int	count_consecutive_quotes(char **q, char **eq)
{
	char	*temp_ptr;
	char	*temp_end;
	int		count;

	temp_ptr = *q;
	temp_end = *eq;
	count = 0;
	while (temp_ptr < temp_end)
	{
		if (*temp_ptr == '"' || *temp_ptr == '\'')
		{
			skip_to_quote_end(&temp_ptr, temp_end, *temp_ptr);
			count++;
			// After skip_to_quote_end, temp_ptr points to the character after the closing quote
			// If we've reached the end, break to avoid counting again
			if (temp_ptr >= temp_end)
				break;
		}
		else
		{
			temp_ptr++;
		}
	}

	return (count);
}

int	process_consecutive_quotes(t_consecutive_quotes_params params)
{
	char	*concatenated;

	concatenated = concatenate_quoted_strings(params.params.input_ptr,
			params.params.input_end, params.env_copy);
	if (! concatenated)
	{
		free_cmd(*params.ret);
		return (1);
	}
	add_argument(params.params.cmd, concatenated, params.params.argc);
	*params.q = *params.params.input_ptr;
	*params.eq = *params.params.input_ptr;
	return (0);
}
