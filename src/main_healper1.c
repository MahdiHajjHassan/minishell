/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_healper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	handle_eof(char *buf, size_t len)
{
	if (len == 0)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		free(buf);
		exit(g_last_exit_status);
	}
	clearerr(stdin);
	return (1);
}

static void	handle_backspace(size_t *len)
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

static char	*resize_buffer(char *buf, size_t *capacity)
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

static char	*init_buffer(size_t *capacity, size_t *len)
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

static int	process_char(int c, char **buf, size_t *capacity, size_t *len)
{
	if (c == EOF)
	{
		handle_eof(*buf, *len);
		(*buf)[*len] = '\0';
		return (0);
	}
	if (c == '\n')
	{
		(*buf)[*len] = '\0';
		return (1);
	}
	if (c == 127 || c == 8)
	{
		handle_backspace(len);
		return (0);
	}
	if (*len + 1 >= *capacity)
	{
		*buf = resize_buffer(*buf, capacity);
		if (!*buf)
			return (-1);
	}
	(*buf)[(*len)++] = c;
	return (0);
}

char	*readline_helper(void)
{
	char	*buf;
	size_t	capacity;
	size_t	len;
	int		c;
	int		result;

	buf = init_buffer(&capacity, &len);
	if (!buf)
		return (NULL);
	while (1)
	{
		c = getchar();
		result = process_char(c, &buf, &capacity, &len);
		if (result == 1)
			return (buf);
		if (result == -1)
			return (NULL);
	}
}

void	init_signals(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
}

void	display_prompt(void)
{
	char	cwd[1024];

	if (isatty(STDIN_FILENO))
	{
		if (get_cwd(cwd, sizeof(cwd)))
		{
			printf("%s> ", cwd);
			fflush(stdout);
		}
	}
}

int	handle_line_input(char **line)
{
	*line = readline_helper();
	if (!*line)
	{
		display_prompt();
		return (1);
	}
	return (0);
}
