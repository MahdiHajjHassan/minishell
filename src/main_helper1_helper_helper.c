/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper1_helper_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_eof_char(char **buf, size_t *len)
{
	handle_eof(*buf, *len);
	(*buf)[*len] = '\0';
	return (0);
}

int	handle_error_char(char **buf)
{
	free(*buf);
	return (-1);
}

int	handle_newline_char(char **buf, size_t *len)
{
	(*buf)[*len] = '\0';
	return (1);
}

int	handle_backspace_char(size_t *len)
{
	handle_backspace(len);
	return (0);
}

int	handle_char_input(char **buf, size_t *capacity, size_t *len, int c)
{
	if (*len + 1 >= *capacity)
	{
		*buf = resize_buffer(*buf, capacity);
		if (!*buf)
			return (-1);
	}
	(*buf)[(*len)++] = c;
	return (0);
}
