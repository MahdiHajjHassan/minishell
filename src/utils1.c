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

void	free_cmd(struct s_cmd *cmd)
{
	if (! cmd)
		return ;
	if (cmd->type == EXEC)
		free_exec_cmd((struct s_execcmd *)cmd);
	else if (cmd->type == REDIR)
		free_redir_cmd((struct s_redircmd *)cmd);
	else if (cmd->type == HEREDOC)
		free_heredoc_cmd((struct s_heredoccmd *)cmd);
	else if (cmd->type == PIPE)
		free_pipe_cmd((struct s_pipecmd *)cmd);
	else if (cmd->type == BACK)
		free_back_cmd((struct s_backcmd *)cmd);
}
