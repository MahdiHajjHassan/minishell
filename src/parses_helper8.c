/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_exec_token(struct s_cmd *ret, t_process_args_params params,
		char **env_copy)
{
	char						*q;
	char						*eq;
	int														tok;
	t_expanded_arg_params	expanded_params;

	tok = get_exec_token(params.input_ptr, params.input_end, &q, &eq);
	if (tok == 0)
		return (0);
	expanded_params.ret = ret;
	expanded_params.params = params;
	expanded_params.env_copy = env_copy;
	expanded_params.q = q;
	expanded_params.eq = eq;
	if (process_expanded_argument(expanded_params))
		return (1);
	return (0);
}

int	process_single_exec_argument(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	char	*q;
	char	*eq;
	char	*processed;

	(void)env_copy;
	if (gettoken(params.input_ptr, params.input_end, &q, &eq) != 'a')
		return (1);
	processed = process_argument(q, eq);
	if (! processed)
	{
		free_cmd(ret);
		return (1);
	}
	add_argument(params.cmd, processed, params.argc);
	return (0);
}

struct s_cmd	*process_arguments(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	while (peek(params.input_ptr, params.input_end, " \t\n\r"))
	{
		gettoken(params.input_ptr, params.input_end, 0, 0);
	}
	if (process_single_exec_argument(ret, params, env_copy))
		return (NULL);
	while (peek(params.input_ptr, params.input_end, " \t\n\r"))
	{
		gettoken(params.input_ptr, params.input_end, 0, 0);
	}
	return (ret);
}

static struct s_cmd	*insert_heredoc_innermost(struct s_cmd *cmd,
		char *delimiter, char *content)
{
	struct s_heredoccmd	*hcmd;

	if (!cmd || cmd->type != HEREDOC)
		return (heredoccmd(cmd, delimiter, content));
	hcmd = (struct s_heredoccmd *)cmd;
	hcmd->cmd = insert_heredoc_innermost(hcmd->cmd, delimiter, content);
	if (!hcmd->cmd)
		return (NULL);
	return ((struct s_cmd *)hcmd);
}

struct s_cmd	*handle_heredoc_token(struct s_cmd *cmd, char *delimiter,
		char **env_copy, int is_quoted)
{
	char	*content;

	content = read_heredoc_content(delimiter, env_copy, is_quoted);
	if (! content)
		return (NULL);
	return (insert_heredoc_innermost(cmd, delimiter, content));
}
