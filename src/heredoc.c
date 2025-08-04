/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_heredoc_line(char *line, char **env_copy, int is_quoted)
{
	char	*expanded_line;

	expanded_line = line;
	if (! is_quoted)
	{
		expanded_line = expand_variables(line, ft_strlen(line), env_copy);
		if (! expanded_line)
		{
			free(line);
			return (NULL);
		}
	}
	return (expanded_line);
}

static char	*handle_heredoc_line(char *content, char *line, char **env_copy,
		int is_quoted)
{
	char	*expanded_line;
	char	*new_content;

	expanded_line = process_heredoc_line(line, env_copy, is_quoted);
	if (! expanded_line)
		return (NULL);
	new_content = append_line_to_content(content, expanded_line);
	free(content);
	free(line);
	if (expanded_line != line)
		free(expanded_line);
	return (new_content);
}

static int	handle_heredoc_line_read(char *line, char *stripped_delimiter)
{
	if (line == (char *)-1)
	{
		print_heredoc_eof_warning(stripped_delimiter);
		return (1);
	}
	if (line == (char *)-2)
		return (2);
	if (check_delimiter_match(line, stripped_delimiter))
	{
		free(line);
		return (1);
	}
	return (0);
}

static char	*process_heredoc_loop(char *content, char *stripped_delimiter,
		char **env_copy, int is_quoted)
{
	char	*line;
	char	*new_content;
	int		result;

	while (1)
	{
		line = read_heredoc_line();
		result = handle_heredoc_line_read(line, stripped_delimiter);
		if (result == 1)
			break ;
		if (result == 2)
			return (NULL);
		new_content = handle_heredoc_line(content, line, env_copy, is_quoted);
		if (! new_content)
			return (NULL);
		content = new_content;
	}
	return (content);
}

char	*read_heredoc_content(char *delimiter, char **env_copy, int is_quoted)
{
	char	*stripped_delimiter;
	char	*content;

	if (! initialize_heredoc_content(&stripped_delimiter, &content, delimiter))
		return (NULL);
	setup_heredoc_signals();
	content = process_heredoc_loop(content,
			stripped_delimiter, env_copy, is_quoted);
	if (! content)
	{
		free(stripped_delimiter);
		return (NULL);
	}
	cleanup_heredoc_content(stripped_delimiter);
	return (content);
}
