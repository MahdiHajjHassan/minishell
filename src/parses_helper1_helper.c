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
