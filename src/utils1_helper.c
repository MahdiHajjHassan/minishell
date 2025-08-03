/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_case(struct s_cmd *cmd)
{
	struct s_redircmd	*rcmd;

	if (! cmd)
		return ;
	rcmd = (struct s_redircmd *)cmd;
	if (rcmd->cmd)
		nulterm(rcmd->cmd);
	if (rcmd->efile)
		*rcmd->efile = 0;
}

void	handle_pipe_case(struct s_cmd *cmd)
{
	struct s_pipecmd	*pcmd;

	if (! cmd)
		return ;
	pcmd = (struct s_pipecmd *)cmd;
	if (pcmd->left)
		nulterm(pcmd->left);
	if (pcmd->right)
		nulterm(pcmd->right);
}

void	handle_back_case(struct s_cmd *cmd)
{
	struct s_backcmd	*bcmd;

	if (! cmd)
		return ;
	bcmd = (struct s_backcmd *)cmd;
	if (bcmd->cmd)
		nulterm(bcmd->cmd);
}

void	handle_heredoc_case(struct s_cmd *cmd)
{
	struct s_heredoccmd	*hcmd;

	if (! cmd)
		return ;
	hcmd = (struct s_heredoccmd *)cmd;
	if (hcmd->cmd)
		nulterm(hcmd->cmd);
}
