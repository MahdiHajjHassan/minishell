/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exec_token(char **input_ptr, char *input_end, char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, q, eq);
	if (tok == 0)
		return (0);
	if (tok != 'a')
	{
		print_missing_file_name();
		return (0);
	}
	return (tok);
}

int	remove_exec_quotes(char **q, char **eq)
{
	int	quote_type;

	quote_type = 0;
	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
		quote_type = '"';
	}
	else if (**q == '\'' && *(*eq - 1) == '\'')
	{
		(*q)++;
		(*eq)--;
		quote_type = '\'';
	}

	return (quote_type);
}

char	*process_argument(char *q, char *eq)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	processed = process_escaped(q, len);
	if (! processed)
	{
		print_malloc_failed();
		return (NULL);
	}
	return (processed);
}

char	*process_argument_with_expansion(char *q, char *eq, char **env_copy,
		int quote_type)
{
	size_t	len;
	char	*processed;
	char	*expanded;

	len = eq - q;
	
	// Only process escape sequences for double-quoted strings
	if (quote_type == '"')
	{
		processed = process_escaped(q, len);
		if (! processed)
		{
			print_malloc_failed();
			return (NULL);
		}
	}
	else
	{
		// For unquoted strings and single-quoted strings, just copy the string
		processed = malloc(len + 1);
		if (! processed)
		{
			print_malloc_failed();
			return (NULL);
		}
		ft_memcpy(processed, q, len);
		processed[len] = '\0';
	}
	
	if (quote_type != '\'')
	{
		// Check if the string contains any variables before expanding
		int has_variables = 0;
		for (size_t i = 0; processed[i]; i++)
		{
			if (processed[i] == '$' && processed[i + 1] && 
				(isalnum(processed[i + 1]) || processed[i + 1] == '_' || processed[i + 1] == '?'))
			{
				has_variables = 1;
				break;
			}
		}
		if (has_variables)
		{
			expanded = expand_variables(processed, ft_strlen(processed), env_copy);
			free(processed);
			if (! expanded)
			{
				print_malloc_failed();
				return (NULL);
			}
			return (expanded);
		}
		else
		{
			return (processed);
		}
	}
	return (processed);
}
