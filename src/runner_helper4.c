/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_left(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->left);
	}
}

void	setup_pipe_right(int *p, struct s_pipecmd *pipecmd)
{
	if (forkk() == 0)
	{
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		runcmd(pipecmd->right);
	}
}

void	run_back_cmd(struct s_cmd *cmd)
{
	struct s_backcmd	*back;

	back = (struct s_backcmd *)cmd;
	if (forkk() == 0)
	{
		if (forkk() == 0)
		{
			runcmd(back->cmd);
		}
		exit(0);
	}
	wait(0);
}
