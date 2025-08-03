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

void	free_heredoc_cmd(struct s_heredoccmd *hcmd)
{
	if (! hcmd)
		return ;
	if (hcmd->cmd)
		free_cmd(hcmd->cmd);
	if (hcmd->delimiter)
	{
		free(hcmd->delimiter);
		hcmd->delimiter = NULL;
	}
	if (hcmd->content)
	{
		free(hcmd->content);
		hcmd->content = NULL;
	}
	free(hcmd);
}
