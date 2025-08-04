/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper9_helper_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirection_cmd_params(t_redir_cmd_params *cmd_params,
		t_redir_file_params params, char *file_or_delimiter, int was_quoted)
{
	cmd_params->ret = params.ret;
	cmd_params->file_or_delimiter = file_or_delimiter;
	cmd_params->env_copy = params.env_copy;
	cmd_params->tok = params.tok;
	cmd_params->was_quoted = was_quoted;
	return (0);
}
