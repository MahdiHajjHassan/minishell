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
	char	quote_char;
	char	*start_pos;

	if (*current_pos >= input_end)
		return (result);
	quote_char = **current_pos;
	start_pos = *current_pos;
	result = process_quoted_segment(*current_pos, input_end, env_copy, result);
	if (! result)
		return (NULL);
	(*current_pos)++;
	while (*current_pos < input_end && **current_pos != quote_char)
		(*current_pos)++;
	if (*current_pos < input_end && **current_pos == quote_char)
		(*current_pos)++;
	else
	{
		*current_pos = start_pos + 1;
		if (*current_pos >= input_end)
			*current_pos = input_end;
	}
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
	char	*prev_pos;

	while (current_pos < input_end)
	{
		prev_pos = current_pos;
		current_pos = skip_whitespace_chars(current_pos, input_end);
		if (current_pos >= input_end)
			break ;
		if (*current_pos == '"' || *current_pos == '\'')
		{
			result = handle_quote_in_loop(&current_pos, input_end, env_copy,
					result);
			if (! result)
				return (NULL);
			if (! advance_position_safely(&current_pos, prev_pos, input_end))
				break ;
		}
		else
			break ;
	}
	return (result);
}
