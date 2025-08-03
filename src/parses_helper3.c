/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, NULL,
			(t_redir_params){O_WRONLY | O_CREAT | O_APPEND, 1}));
}

struct s_cmd	*init_exec_cmd(void)
{
	struct s_cmd	*ret;

	ret = execcmd();
	return (ret);
}
