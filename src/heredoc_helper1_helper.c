/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper1_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_read_error(ssize_t bytes_read, int i)
{
	if (bytes_read == 0)
	{
		if (i == 0)
			return (-1);
		return (1);
	}
	if (bytes_read == -1)
	{
		if (errno == EINTR)
			return (-2);
		return (-1);
	}
	return (0);
}

int	read_character(int *c)
{
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, c, 1);
	return (bytes_read);
}

char	*process_line_buffer(char *buffer, int i)
{
	char	*line;

	buffer[i] = '\0';
	line = ft_strdup(buffer);
	return (line);
}
