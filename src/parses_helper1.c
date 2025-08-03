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

	output = malloc(len + 1);
	if (!output)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
		{
			// Process escape sequences
			if (input[i + 1] == '\\')
			{
				output[j] = '\\';
				i += 2;
			}
			else if (input[i + 1] == '"')
			{
				output[j] = '"';
				i += 2;
			}
			else
			{
				// Keep the backslash and the next character as literal
				output[j] = '\\';
				j++;
				output[j] = input[i + 1];
				i += 2;
			}
		}
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

char	*process_filename(char *q, char *eq, char **env_copy)
{
	size_t	len;
	char	*processed;
	char	*expanded;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
	{
		print_malloc_failed();
		return (NULL);
	}
	expanded = expand_variables(processed, ft_strlen(processed), env_copy);
	free(processed);
	if (!expanded)
	{
		print_malloc_failed();
		return (NULL);
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
	else if (tok == 'H')
		return (handle_heredoc_token(cmd, file, NULL, 0));
	return (cmd);
}
