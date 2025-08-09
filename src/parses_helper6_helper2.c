/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_double_quotes(char *q, size_t len)
{
	char	*processed;

	processed = process_escaped(q, len);
	if (! processed)
	{
		print_malloc_failed();
		return (NULL);
	}
	return (processed);
}

static char	*handle_single_quotes(char *q, size_t len)
{
	char	*processed;

	processed = malloc(len + 1);
	if (! processed)
	{
		print_malloc_failed();
		return (NULL);
	}
	ft_memcpy(processed, q, len);
	processed[len] = '\0';
	return (processed);
}

char	*process_quoted_string(char *q, size_t len, int quote_type)
{
	char	*processed;

	if (quote_type == '"')
		processed = handle_double_quotes(q, len);
	else
		processed = handle_single_quotes(q, len);
	if (! processed)
		return (NULL);
	if (quote_type == 0)
		return (remove_embedded_quotes(processed, len));
	return (processed);
}

int	check_for_variables(char *processed)
{
	size_t	i;

	i = 0;
	while (processed[i])
	{
		if (processed[i] == '$' && processed[i + 1]
			&& (ft_isalnum(processed[i + 1]) || processed[i + 1] == '_'
				|| processed[i + 1] == '?'))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

char	*expand_if_needed(char *processed, char **env_copy)
{
	char	*expanded;

	expanded = expand_variables(processed, ft_strlen(processed), env_copy);
	free(processed);
	if (! expanded)
	{
		print_malloc_failed();
		return (NULL);
	}
	return (expanded);
}
