/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unclosed_quotes(const char *line)
{
	char	quote;

	quote = 0;
	while (*line)
	{
		if (!quote)
		{
			if (*line == '"' || *line == '\'')
				quote = *line;
		}
		else if (quote && *line == quote)
		{
			quote = 0;
		}
		line++;
	}
	if (quote)
		return (1);
	return (0);
}

const char	*check_double_quote_pattern(const char *line)
{
	line++;
	while (*line && *line != '"')
		line++;
	if (*line == '"')
		line++;
	return (line);
}

const char	*check_single_quote_pattern(const char *line)
{
	line++;
	while (*line && *line != '\'')
		line++;
	if (*line == '\'')
		line++;
	return (line);
}

int	check_quote_patterns(const char *line)
{
	while (*line)
	{
		if (*line == '"')
			line = check_double_quote_pattern(line);
		else if (*line == '\'')
			line = check_single_quote_pattern(line);
		else
			line++;
	}
	return (0);
}

int	has_malformed_quotes(const char *line)
{
	if (check_unclosed_quotes(line))
		return (1);
	return (check_quote_patterns(line));
}
