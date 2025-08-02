/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (! cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct s_cmd *)cmd);
}
