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

	rcmd = (struct s_redircmd *)cmd;
	nulterm(rcmd->cmd);
	*rcmd->efile = 0;
}

void	handle_pipe_case(struct s_cmd *cmd)
{
	struct s_pipecmd	*pcmd;

	pcmd = (struct s_pipecmd *)cmd;
	nulterm(pcmd->left);
	nulterm(pcmd->right);
}

void	handle_list_case(struct s_cmd *cmd)
{
	struct s_listcmd	*lcmd;

	lcmd = (struct s_listcmd *)cmd;
	nulterm(lcmd->left);
	nulterm(lcmd->right);
}

void	handle_back_case(struct s_cmd *cmd)
{
	struct s_backcmd	*bcmd;

	bcmd = (struct s_backcmd *)cmd;
	nulterm(bcmd->cmd);
}
