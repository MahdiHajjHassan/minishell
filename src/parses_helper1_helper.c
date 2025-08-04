/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper1_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collapse_consecutive_quotes(const char *input)
{
	size_t	len;
	char	*output;
	size_t	i;
	size_t	j;

	len = ft_strlen(input);
	output = malloc(len + 1);
	if (!output)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '"' && input[i + 1] == '"')
		{
			i += 2;
		}
		else
		{
			output[j++] = input[i++];
		}
	}
	output[j] = '\0';
	return (output);
}

char	*process_filename(char *q, char *eq, char **env_copy)
{
	size_t	len;
	char	*processed;
	char	*expanded;
	char	*collapsed;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
		return (handle_processing_error(NULL));
	collapsed = collapse_consecutive_quotes(processed);
	free(processed);
	if (!collapsed)
		return (handle_processing_error(NULL));
	expanded = expand_variables(collapsed, ft_strlen(collapsed), env_copy);
	free(collapsed);
	if (!expanded)
		return (handle_processing_error(NULL));
	return (expanded);
}

char	*process_heredoc_delimiter(char *q, char *eq, char **env_copy, int is_quoted)
{
	size_t	len;
	char	*processed;
	char	*expanded;
	char	*collapsed;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
		return (handle_processing_error(NULL));
	collapsed = collapse_consecutive_quotes(processed);
	free(processed);
	if (!collapsed)
		return (handle_processing_error(NULL));
	if (is_quoted)
	{
		char	*result;

		result = ft_strdup(collapsed);
		free(collapsed);
		return (result);
	}
	expanded = expand_variables(collapsed, ft_strlen(collapsed), env_copy);
	free(collapsed);
	if (!expanded)
		return (handle_processing_error(NULL));
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
