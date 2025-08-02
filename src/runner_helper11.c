/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper11.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_children(pid_t pid1, pid_t pid2,
	int *status1, int *status2)
{
	waitpid(pid1, status1, 0);
	waitpid(pid2, status2, 0);
}

void	handle_pipe_status(int status2)
{
	if (WIFEXITED(status2))
		set_exit_status(WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
	{
		if (WTERMSIG(status2) == SIGINT)
			set_exit_status(130);
		else if (WTERMSIG(status2) == SIGQUIT)
			set_exit_status(131);
		else
			set_exit_status(128 + WTERMSIG(status2));
	}
	else
		set_exit_status(1);
}
