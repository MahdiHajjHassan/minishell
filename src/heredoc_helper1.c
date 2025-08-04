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

static int	process_character_loop(char *buffer, int *i)
{
	int		c;
	int		result;

	while (*i < 4095)
	{
		result = read_character(&c);
		if (result != 1)
		{
			result = handle_read_error(result, *i);
			if (result == -1)
				return (-1);
			if (result == -2)
				return (-2);
			if (result == 1)
				return (1);
		}
		if (get_exit_status() == 130)
			return (-2);
		if (c == '\n')
			return (1);
		buffer[(*i)++] = (char)c;
	}
	return (0);
}

char	*read_line_without_history(void)
{
	char	buffer[4096];
	char	*line;
	int		i;
	int		result;

	i = 0;
	result = process_character_loop(buffer, &i);
	if (result == -1)
		return ((char *)-1);
	if (result == -2)
		return ((char *)-2);
	line = process_line_buffer(buffer, i);
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
