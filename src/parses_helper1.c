/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_output_buffer(size_t len)
{
	char	*output;

	output = malloc(len + 1);
	if (!output)
		return (NULL);
	return (output);
}

void	handle_escape_sequence(const char *input,
		size_t *i, char *output, size_t *j)
{
	if (input[*i + 1] == '\\')
	{
		output[*j] = '\\';
		*i += 2;
	}
	else if (input[*i + 1] == '"')
	{
		output[*j] = '"';
		*i += 2;
	}
	else
	{
		output[*j] = '\\';
		(*j)++;
		output[*j] = input[*i + 1];
		*i += 2;
	}
}

static void	handle_regular_character(const char *input,
		char *output, size_t *i, size_t *j)
{
	output[*j] = input[*i];
	(*i)++;
}

char	*process_escaped(const char *input, size_t len)
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
			handle_regular_character(input, output, &i, &j);
		j++;
	}
	output[j] = '\0';
	return (output);
}

int	get_redir_token(char **input_ptr, char *input_end,
			char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, NULL, NULL);
	if (gettoken(input_ptr, input_end, q, eq) != 'a')
	{
		print_missing_file_name();
		return (0);
	}
	return (tok);
}
