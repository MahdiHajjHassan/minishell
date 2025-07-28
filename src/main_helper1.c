/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_eof(char *buf, size_t len)
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
