/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*initialize_quote_result(char *processed, size_t len)
{
	char	*result;

	result = malloc(len + 1);
	if (!result)
	{
		free(processed);
		print_malloc_failed();
		return (NULL);
	}
	return (result);
}

char	*filter_quotes_from_string(char *processed, size_t len)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = initialize_quote_result(processed, len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (processed[i] != '"' && processed[i] != '\'')
		{
			result[j] = processed[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	free(processed);
	return (result);
}

char	*remove_embedded_quotes(char *processed, size_t len)
{
	if (has_dollar_quote_pattern(processed, len))
		return (process_dollar_quote_patterns(processed, len));
	return (filter_quotes_from_string(processed, len));
}

int	has_dollar_quote_pattern(char *processed, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len - 1)
	{
		if (processed[i] == '$' && (processed[i + 1] == '"'
				|| processed[i + 1] == '\''))
			return (1);
		i++;
	}
	return (0);
}

void	process_dollar_quote_content(t_quote_params *params)
{
	char	quote_char;
	size_t	quote_start;

	quote_char = params->processed[*(params->i) + 1];
	quote_start = *(params->i) + 2;
	*(params->i) = quote_start;
	while (*(params->i) < params->len
		&& params->processed[*(params->i)] != quote_char)
	{
		params->result[*(params->j)] = params->processed[*(params->i)];
		(*(params->j))++;
		(*(params->i))++;
	}
	if (*(params->i) < params->len
		&& params->processed[*(params->i)] == quote_char)
		(*(params->i))++;
}
