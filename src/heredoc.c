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

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
}

void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "\n", 1);
	clean_exit(130);
}

char	*read_line_without_history(void)
{
	char	buffer[4096];
	char	*line;
	int		i;
	int		c;

	i = 0;
	while (i < 4095)
	{
		c = getchar();
		if (c == EOF)
		{
			if (i == 0)
				return (NULL);
			break ;
		}
		if (c == '\n')
			break ;
		buffer[i++] = (char)c;
	}
	buffer[i] = '\0';
	line = ft_strdup(buffer);
	return (line);
}

char	*append_line_to_content(char *content, char *line)
{
	size_t	content_len;
	size_t	line_len;
	char	*new_content;

	content_len = ft_strlen(content);
	line_len = ft_strlen(line);
	new_content = malloc(content_len + line_len + 2);
	if (! new_content)
		return (NULL);
	ft_strcpy(new_content, content);
	ft_strcpy(new_content + content_len, line);
	new_content[content_len + line_len] = '\n';
	new_content[content_len + line_len + 1] = '\0';
	return (new_content);
}

char	*read_heredoc_content(char *delimiter, char **env_copy, int is_quoted)
{
	char	*stripped_delimiter;
	char	*content;
	char	*line;
	char	*new_content;
	size_t	delimiter_len;
	char	*expanded_line;

	stripped_delimiter = ft_strdup(delimiter);
	content = ft_strdup("");
	if (! content)
	{
		free(stripped_delimiter);
		return (NULL);
	}
	delimiter_len = ft_strlen(stripped_delimiter);
	setup_heredoc_signals();
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_without_history();
		if (! line)
		{
			ft_fprintf_stderr("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				delimiter);
			break ;
		}
		if (ft_strlen(line) == delimiter_len
			&& ft_strncmp(line, stripped_delimiter, delimiter_len) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = line;
		if (! is_quoted)
		{
			expanded_line = expand_variables(line, ft_strlen(line), env_copy);
			if (! expanded_line)
			{
				free(line);
				free(content);
				free(stripped_delimiter);
				return (NULL);
			}
		}
		else
		{
			expanded_line = line;
		}
		new_content = append_line_to_content(content, expanded_line);
		free(content);
		free(line);
		if (expanded_line != line)
			free(expanded_line);
		if (! new_content)
		{
			free(stripped_delimiter);
			return (NULL);
		}
		content = new_content;
	}
	init_signals();
	free(stripped_delimiter);
	return (content);
} 