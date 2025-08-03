/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quoted_string(char **current_pos, char *input_end,
		char **env_copy, char *result)
{
	result = process_quoted_segment(*current_pos, input_end, env_copy, result);
	if (! result)
		return (NULL);
	*current_pos = *current_pos + 1;
	while (*current_pos < input_end && **current_pos != '"'
		&& **current_pos != '\'')
		(*current_pos)++;
	if (*current_pos < input_end)
		(*current_pos)++;
	return (result);
}

char	*initialize_concatenation(char **input_ptr)
{
	char	*result;

	(void)input_ptr;
	result = ft_strdup("");
	if (! result)
		return (NULL);
	return (result);
}

char	*process_concatenation_loop(char *result, char *current_pos,
		char *input_end, char **env_copy)
{
	while (current_pos < input_end)
	{
		current_pos = skip_whitespace_chars(current_pos, input_end);
		if (current_pos >= input_end)
			break ;
		if (*current_pos == '"' || *current_pos == '\'')
		{
			result = handle_quoted_string(&current_pos, input_end, env_copy,
					result);
			if (! result)
				return (NULL);
		}
		else
		{
			break ;
		}
	}
	return (result);
}

char	*concatenate_quoted_strings(char **input_ptr, char *input_end,
		char **env_copy)
{
	char	*result;
	char	*current_pos;

	result = initialize_concatenation(input_ptr);
	if (! result)
		return (NULL);
	current_pos = *input_ptr;
	result = process_concatenation_loop(result, current_pos, input_end,
			env_copy);
	if (! result)
		return (NULL);
	*input_ptr = current_pos;
	return (result);
}

int	process_expanded_argument(t_expanded_arg_params params)
{
	char	*processed;
	int		quote_type;

	quote_type = remove_exec_quotes(&params.q, &params.eq);
	processed = process_argument_with_expansion(params.q,
			params.eq, params.env_copy, quote_type);
	if (! processed)
	{
		free_cmd(params.ret);
		return (1);
	}
	add_argument(params.params.cmd, processed, params.params.argc);
	return (0);
}
