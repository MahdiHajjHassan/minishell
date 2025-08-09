/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper7_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quote_in_loop(char **current_pos, char *input_end,
		char **env_copy, char *result)
{
	result = handle_quoted_string(current_pos, input_end, env_copy, result);
	if (! result)
		return (NULL);
	return (result);
}

char	*advance_position_safely(char **current_pos, char *prev_pos,
		char *input_end)
{
	if (*current_pos == prev_pos)
	{
		(*current_pos)++;
		if (*current_pos >= input_end)
			return (NULL);
	}
	return (*current_pos);
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
