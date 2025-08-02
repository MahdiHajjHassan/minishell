/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper9.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_filename_token(int filename_tok, struct s_cmd **ret,
		t_process_args_params params)
{
	(void)params;
	if (filename_tok != 'a')
	{
		print_missing_file_name();
		free_cmd(*ret);
		return (1);
	}
	return (0);
}

int	check_quoted_status(char **q, char **eq)
{
	int	was_quoted;

	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
		was_quoted = 1;
		return (was_quoted);
	}
	else if (**q == '\'' && *(*eq - 1) == '\'')
	{
		(*q)++;
		(*eq)--;
		was_quoted = 1;
		return (was_quoted);
	}
	return (0);
}

int	create_redirection_cmd(t_redir_cmd_params params)
{
	if (params.tok == '<')
		*params.ret = apply_input_redir(*params.ret, params.file_or_delimiter);
	else if (params.tok == '>')
		*params.ret = apply_output_redir(*params.ret, params.file_or_delimiter);
	else if (params.tok == '+')
		*params.ret = apply_append_redir(*params.ret, params.file_or_delimiter);
	else if (params.tok == 'h')
		*params.ret = handle_heredoc_token(*params.ret,
				params.file_or_delimiter, params.env_copy, params.was_quoted);
	return (0);
}

int	process_redirection_file(t_redir_file_params params)
{
	char				*file_or_delimiter;
	int					was_quoted;
	t_redir_cmd_params	cmd_params;

	was_quoted = check_quoted_status(params.q, params.eq);
	file_or_delimiter = process_filename(*params.q, *params.eq,
			params.env_copy);
	if (!file_or_delimiter)
	{
		free_cmd(*params.ret);
		return (1);
	}
	cmd_params.ret = params.ret;
	cmd_params.file_or_delimiter = file_or_delimiter;
	cmd_params.env_copy = params.env_copy;
	cmd_params.tok = params.tok;
	cmd_params.was_quoted = was_quoted;
	create_redirection_cmd(cmd_params);
	return (0);
}
