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
		clean_exit(g_sig.exit_status);
	}
	clearerr(stdin);
	return (1);
}

void	init_signals(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
}



int	handle_line_input(char **line)
{
	g_sig.sigint = 0;
	*line = readline("minishell$ ");
	if (!*line)
	{
		printf("exit\n");
		clean_exit(0);
	}
	if (**line)
		add_history(*line);
	return (0);
}
