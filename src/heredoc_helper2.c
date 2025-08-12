/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_delimiter_match(char *line, char *stripped_delimiter)
{
	size_t	delimiter_len;

	delimiter_len = ft_strlen(stripped_delimiter);
	return (ft_strlen(line) == delimiter_len
		&& ft_strncmp(line, stripped_delimiter, delimiter_len) == 0);
}

char	*read_heredoc_line(void)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = read_line_without_history();
	return (line);
}
