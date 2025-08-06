/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (0);
	ecmd = (struct s_execcmd *)cmd;
	return (!ecmd->av[0] || ft_strlen(ecmd->av[0]) == 0);
}

int	handle_redirection_token_type(t_token_type_params token_params)
{
	t_redir_token_params	redir_params;

	redir_params.ret = token_params.ret;
	redir_params.params = token_params.params;
	redir_params.env_copy = token_params.env_copy;
	redir_params.tok = token_params.tok;
	redir_params.q = token_params.q;
	redir_params.eq = token_params.eq;
	return (handle_redirection_token(redir_params));
}

int	is_escaped(const char *s, const char *start)
{
	(void)s;
	(void)start;
	return (0);
}
