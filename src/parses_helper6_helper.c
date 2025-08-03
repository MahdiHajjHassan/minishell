/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_quoted_string(char *q, size_t len, int quote_type)
{
	char	*processed;

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
		processed = malloc(len + 1);
		if (! processed)
		{
			print_malloc_failed();
			return (NULL);
		}
		ft_memcpy(processed, q, len);
		processed[len] = '\0';
	}
	return (processed);
}

static int	check_for_variables(char *processed)
{
	size_t	i;

	i = 0;
	while (processed[i])
	{
		if (processed[i] == '$' && processed[i + 1]
			&& (isalnum(processed[i + 1]) || processed[i + 1] == '_'
				|| processed[i + 1] == '?'))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static char	*expand_if_needed(char *processed, char **env_copy)
{
	char	*expanded;
	size_t	original_len;

	original_len = ft_strlen(processed);
	expanded = expand_variables(processed, original_len, env_copy);
	
	if (! expanded)
	{
		free(processed);
		print_malloc_failed();
		return (NULL);
	}
	
	// Check if expansion resulted in an empty string when it shouldn't
	if (ft_strlen(expanded) == 0 && original_len > 0)
	{
		// This is likely a bug - return the original string instead
		free(expanded);
		return (processed);
	}
	
	free(processed);
	return (expanded);
}

char	*process_argument_with_expansion(char *q, char *eq, char **env_copy,
		int quote_type)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	
	// DEBUG
	// printf("DEBUG process_argument_with_expansion: len=%zu, quote_type=%c, content='%.*s'\n", 
	//        len, quote_type ? quote_type : '0', (int)len, q);
	
	processed = process_quoted_string(q, len, quote_type);
	if (! processed)
		return (NULL);
		
	// DEBUG
	// printf("DEBUG after process_quoted_string: processed='%s'\n", processed);
	
	if (quote_type != '\'')
	{
		if (check_for_variables(processed))
		{
			// DEBUG
			// printf("DEBUG: Variables found, expanding...\n");
			return (expand_if_needed(processed, env_copy));
		}
	}
	return (processed);
}
