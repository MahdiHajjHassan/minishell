/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper5.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_double_quotes(char *q, size_t len)
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

char	*handle_single_quotes(char *q, size_t len)
{
	char	*processed;

	processed = malloc(len + 1);
	if (!processed)
	{
		print_malloc_failed();
		return (NULL);
	}
	ft_memcpy(processed, q, len);
	processed[len] = '\0';
	return (processed);
}

char	*process_unquoted_escapes(const char *input, size_t len)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = malloc(len + 1);
	if (!output)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
			handle_escape_sequence(input, &i, output, &j);
		else
		{
			output[j] = input[i];
			i++;
		}
		j++;
	}
	output[j] = '\0';
	return (output);
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
