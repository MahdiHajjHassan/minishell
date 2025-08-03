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
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		free(buf);
		clean_exit(0);
	}
	clearerr(stdin);
	return (1);
}

void	init_signals(void)
{
}

int	handle_line_input(char **line)
{
	*line = readline("minishell$ ");
	if (! *line)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (1);
	}
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (0);
}
