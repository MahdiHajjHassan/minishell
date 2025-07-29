/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
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

	i = 0;
	while (ecmd->av[i])
	{
		free(ecmd->av[i]);
		i++;
	}
	free(ecmd);
}

void	free_redir_cmd(struct s_redircmd *rcmd)
{
	free_cmd(rcmd->cmd);
	free(rcmd->file);
	free(rcmd->efile);
	free(rcmd);
}

void	free_pipe_cmd(struct s_pipecmd *pcmd)
{
	free_cmd(pcmd->left);
	free_cmd(pcmd->right);
	free(pcmd);
}

void	free_list_cmd(struct s_listcmd *lcmd)
{
	free_cmd(lcmd->left);
	free_cmd(lcmd->right);
	free(lcmd);
}

void	free_back_cmd(struct s_backcmd *bcmd)
{
	free_cmd(bcmd->cmd);
	free(bcmd);
}

void	free_heredoc_cmd(struct s_heredoccmd *hcmd)
{
	if (!hcmd)
		return ;
	
	free_cmd(hcmd->cmd);
	
	if (hcmd->delimiter)
		free(hcmd->delimiter);
	
	if (hcmd->content)
		free(hcmd->content);
	
	free(hcmd);
}
