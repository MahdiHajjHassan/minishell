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

/* Backslash processing removed - not supported in this minishell */
char	*process_escaped(const char *input, size_t len)
{
	char	*output;
	size_t	i;

	output = malloc(len + 1);
	if (!output)
		return (NULL);
	i = 0;
	while (i < len)
	{
		output[i] = input[i];
		i++;
	}
	output[i] = '\0';
	return (output);
}

int	get_redir_token(char **input_ptr, char *input_end,
			char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, NULL, NULL);
	if (gettoken(input_ptr, input_end, q, eq) != 'a')
	{
		ft_fprintf_stderr("minishell: missing file name\n");
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
		ft_fprintf_stderr("minishell: malloc failed\n");
		return (NULL);
	}
	expanded = expand_variables(processed, ft_strlen(processed), env_copy);
	free(processed);
	if (!expanded)
	{
		ft_fprintf_stderr("minishell: malloc failed\n");
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
		return (handle_heredoc_token(cmd, file));
	return (cmd);
}
