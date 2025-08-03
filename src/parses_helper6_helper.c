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
	char	*result;
	size_t	i;
	size_t	j;

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
	
	// If quote_type is 0 (no outer quotes), remove embedded quotes
	// If quote_type is not 0 (already quoted), preserve inner quotes
	if (quote_type == 0)
	{
		result = malloc(len + 1);
		if (!result)
		{
			free(processed);
			print_malloc_failed();
			return (NULL);
		}
		
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
	char	*original_copy;

	original_copy = ft_strdup(processed);
	expanded = expand_variables(processed, ft_strlen(processed), env_copy);
	free(processed);
	if (! expanded)
	{
		print_malloc_failed();
		free(original_copy);
		return (NULL);
	}
	if (ft_strlen(expanded) == 0)
	{
		free(expanded);
		return (original_copy);
	}
	free(original_copy);
	return (expanded);
}

char	*process_argument_with_expansion(char *q, char *eq, char **env_copy,
		int quote_type)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	processed = process_quoted_string(q, len, quote_type);
	if (! processed)
		return (NULL);
	if (quote_type != '\'')
	{
		if (check_for_variables(processed))
		{
			return (expand_if_needed(processed, env_copy));
		}
	}
	return (processed);
}
