/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_argument(struct s_execcmd *cmd, char *processed, int *argc)
{
	if (*argc >= MAXARGS)
	{
		fprintf(stderr, "too many args\n");
		wtf();
	}
	cmd->av[*argc] = processed;
	cmd->eav[*argc] = cmd->av[*argc] + strlen(processed);
	(*argc)++;
}

void	finalize_exec_cmd(struct s_execcmd *cmd, int argc)
{
	cmd->av[argc] = 0;
	cmd->eav[argc] = 0;
}

struct s_cmd	*process_arguments(struct s_cmd *ret,
			t_process_args_params params)
{
	char	*q;
	char	*eq;
	int		tok;
	char	*processed;

	while (!peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = get_exec_token(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		remove_exec_quotes(&q, &eq);
		processed = process_argument(q, eq);
		add_argument(params.cmd, processed, params.argc);
		ret = parse_redirs(ret, params.input_ptr, params.input_end);
	}
	return (ret);
}
