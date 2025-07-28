/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper1_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_backspace(size_t *len)
{
	if (*len > 0)
	{
		(*len)--;
		if (isatty(STDIN_FILENO))
		{
			printf("\b \b");
			fflush(stdout);
		}
	}
}

char	*resize_buffer(char *buf, size_t *capacity)
{
	char	*new_buf;

	*capacity *= 2;
	new_buf = realloc(buf, *capacity);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	return (new_buf);
}

char	*init_buffer(size_t *capacity, size_t *len)
{
	char	*buf;

	buf = NULL;
	*capacity = 128;
	*len = 0;
	buf = malloc(*capacity);
	if (!buf)
		return (NULL);
	display_prompt();
	return (buf);
}

int	process_char(int c, char **buf, size_t *capacity, size_t *len)
{
	if (c == EOF)
		return (handle_eof_char(buf, len));
	if (c == -1)
		return (handle_error_char(buf));
	if (c == '\n')
		return (handle_newline_char(buf, len));
	if (c == 127 || c == 8)
		return (handle_backspace_char(len));
	return (handle_char_input(buf, capacity, len, c));
}
