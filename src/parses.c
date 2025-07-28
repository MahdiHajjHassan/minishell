/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*parse_pipe(char **input_ptr, char *input_end)
{
	struct s_cmd	*cmd;

	cmd = parseexec(input_ptr, input_end);
	if (peek(input_ptr, input_end, "|"))
	{
		gettoken(input_ptr, input_end, NULL, NULL);
		cmd = pipecmd(cmd, parse_pipe(input_ptr, input_end));
	}
	return (cmd);
}

struct s_cmd	*parse_line(char **input_ptr, char *input_end)
{
	struct s_cmd	*cmd;

	cmd = parse_pipe(input_ptr, input_end);
	if (peek(input_ptr, input_end, "&"))
	{
		gettoken(input_ptr, input_end, NULL, NULL);
		cmd = backcmd(cmd);
		if (!peek(input_ptr, input_end, "\0"))
		{
			return (parse_line(input_ptr, input_end));
		}
	}
	else if (peek(input_ptr, input_end, ";"))
	{
		gettoken(input_ptr, input_end, NULL, NULL);
		cmd = listcmd(cmd, parse_line(input_ptr, input_end));
	}
	return (cmd);
}

struct s_cmd	*parse_block(char **input_ptr, char *input_end)
{
	struct s_cmd	*cmd;

	if (!peek(input_ptr, input_end, "("))
	{
		fprintf(stderr, "missing (\n");
		wtf();
	}
	gettoken(input_ptr, input_end, NULL, NULL);
	cmd = parse_line(input_ptr, input_end);
	if (!peek(input_ptr, input_end, ")"))
	{
		fprintf(stderr, "missing )\n");
		wtf();
	}
	gettoken(input_ptr, input_end, NULL, NULL);
	cmd = parse_redirs(cmd, input_ptr, input_end);
	return (cmd);
}

struct s_cmd	*parse_redirs(struct s_cmd *cmd, char **input_ptr,
				char *input_end)
{
	int		tok;
	char	*q;
	char	*eq;
	char	*file;

	while (peek(input_ptr, input_end, "<>"))
	{
		tok = get_redir_token(input_ptr, input_end, &q, &eq);
		remove_redir_quotes(&q, &eq);
		file = process_filename(q, eq);
		cmd = handle_redir_token(cmd, tok, file);
	}
	return (cmd);
}

struct s_cmd	*parseexec(char **input_ptr, char *input_end)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	int					argc;

	argc = 0;
	if (peek(input_ptr, input_end, "("))
		return (parse_block(input_ptr, input_end));
	ret = init_exec_cmd();
	cmd = (struct s_execcmd *)ret;
	ret = parse_redirs(ret, input_ptr, input_end);
	ret = process_arguments(ret,
			(t_process_args_params){cmd, input_ptr, input_end, &argc});
	finalize_exec_cmd(cmd, argc);
	return (ret);
}
