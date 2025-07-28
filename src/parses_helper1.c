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

char	*process_escaped(const char *input, size_t len)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = init_output_buffer(len);
	i = 0;
	j = 0;
	if (!output)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
		{
			handle_escape_sequence(input, &i, output, &j);
		}
		else
		{
			output[j++] = input[i++];
		}
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
		fprintf(stderr, "missing file name\n");
		wtf();
	}
	return (tok);
}

char	*process_filename(char *q, char *eq)
{
	size_t	len;
	char	*processed;
	char	*expanded;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
	{
		fprintf(stderr, "malloc failed\n");
		wtf();
	}
	expanded = expand_variables(processed, strlen(processed));
	free(processed);
	if (!expanded)
	{
		fprintf(stderr, "malloc failed\n");
		wtf();
	}
	return (expanded);
}

struct s_cmd	*handle_redir_token(struct s_cmd *cmd, int tok, char *file)
{
	if (tok == '<')
		return (apply_input_redir(cmd, file));
	else if (tok == '>')
		return (apply_output_redir(cmd, file));
	else if (tok == '+')
		return (apply_append_redir(cmd, file));
	return (cmd);
}
