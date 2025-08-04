/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
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
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
}

void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "\n", 1);
	set_exit_status(130);
}

char	*read_line_without_history(void)
{
	char	buffer[4096];
	char	*line;
	int		i;
	int		c;
	ssize_t	bytes_read;

	i = 0;
	while (i < 4095)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read == 0)
		{
			if (i == 0)
				return ((char *)-1);
			break ;
		}
		if (bytes_read == -1)
		{
			if (errno == EINTR)
				return ((char *)-2);
			return ((char *)-1);
		}
		if (get_exit_status() == 130)
			return ((char *)-2);
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

int	check_delimiter_match(char *line, char *stripped_delimiter)
{
	size_t	delimiter_len;

	delimiter_len = ft_strlen(stripped_delimiter);
	return (ft_strlen(line) == delimiter_len
		&& ft_strncmp(line, stripped_delimiter, delimiter_len) == 0);
}
