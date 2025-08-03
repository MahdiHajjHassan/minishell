/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_escape_char(char c)
{
	if (c == 'n')
		return ('\n');
	else if (c == 't')
		return ('\t');
	else if (c == 'r')
		return ('\r');
	else if (c == 'v')
		return ('\v');
	else if (c == 'b')
		return ('\b');
	else if (c == 'f')
		return ('\f');
	else if (c == 'a')
		return ('\a');
	else if (c == '\\')
		return ('\\');
	else
		return (c);
}

void	handle_escape_char_sequence(const char *input, size_t *i,
				char *output, size_t *j)
{
	output[(*j)++] = get_escape_char(input[*i + 1]);
	*i += 2;
}

void	remove_redir_quotes(char **q, char **eq)
{
	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
	}
}

struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, NULL,
			(t_redir_params){O_RDONLY, 0}));
}

struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, NULL,
			(t_redir_params){O_WRONLY | O_CREAT | O_TRUNC, 1}));
}
