/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                          :+:      :+:    :+:   */
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

	if (!cmd)
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

	if (!cmd)
		return ;
	
	pcmd = (struct s_pipecmd *)cmd;
	if (pcmd->left)
		nulterm(pcmd->left);
	if (pcmd->right)
		nulterm(pcmd->right);
}

/* List command handling removed - semicolon not supported in this minishell */
void	handle_list_case(struct s_cmd *cmd)
{
	(void)cmd;
}

void	handle_back_case(struct s_cmd *cmd)
{
	struct s_backcmd	*bcmd;

	if (!cmd)
		return ;
	
	bcmd = (struct s_backcmd *)cmd;
	if (bcmd->cmd)
		nulterm(bcmd->cmd);
}

void	handle_heredoc_case(struct s_cmd *cmd)
{
	struct s_heredoccmd	*hcmd;

	if (!cmd)
		return ;
	
	hcmd = (struct s_heredoccmd *)cmd;
	if (hcmd->cmd)
		nulterm(hcmd->cmd);
}

void	free_cmd(struct s_cmd *cmd)
{
	if (!cmd)
		return ;
	
	if (cmd->type == EXEC)
		free_exec_cmd((struct s_execcmd *)cmd);
	else if (cmd->type == REDIR)
		free_redir_cmd((struct s_redircmd *)cmd);
	else if (cmd->type == HEREDOC)
		free_heredoc_cmd((struct s_heredoccmd *)cmd);
	else if (cmd->type == PIPE)
		free_pipe_cmd((struct s_pipecmd *)cmd);
	/* List command handling removed - semicolon not supported in this minishell */
	else if (cmd->type == BACK)
		free_back_cmd((struct s_backcmd *)cmd);
}
