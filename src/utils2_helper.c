/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exec_cmd(struct s_execcmd *ecmd)
{
	int	i;

	if (! ecmd)
		return ;
	i = 0;
	while (ecmd->av[i])
	{
		if (ecmd->av[i])
		{
			free(ecmd->av[i]);
			ecmd->av[i] = NULL;
		}
		i++;
	}
	free(ecmd);
}

void	free_redir_cmd(struct s_redircmd *rcmd)
{
	if (! rcmd)
		return ;
	if (rcmd->cmd)
		free_cmd(rcmd->cmd);
	if (rcmd->file)
	{
		free(rcmd->file);
		rcmd->file = NULL;
	}
	if (rcmd->efile)
	{
		free(rcmd->efile);
		rcmd->efile = NULL;
	}
	free(rcmd);
}

void	free_pipe_cmd(struct s_pipecmd *pcmd)
{
	if (! pcmd)
		return ;
	if (pcmd->left)
		free_cmd(pcmd->left);
	if (pcmd->right)
		free_cmd(pcmd->right);
	free(pcmd);
}

void	free_back_cmd(struct s_backcmd *bcmd)
{
	if (! bcmd)
		return ;
	if (bcmd->cmd)
		free_cmd(bcmd->cmd);
	free(bcmd);
}
