/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int	handle_tokenize(char *line, struct s_cmd **cmd)
{
	*cmd = tokenize(line);
	if (!*cmd)
	{
		free(line);
		return (1);
	}
	return (0);
}



void	execute_cmd(struct s_cmd *cmd)
{
	int	status;

	g_sig.pid = forkk();
	if (g_sig.pid == 0)
	{
		runcmd(cmd);
		exit(0);
	}
	wait(&status);
	handle_child_status(status);
}
