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
	struct s_cmd	*result;

	if (params.tok == '<')
	{
		result = replace_input_redir(*params.ret, params.file_or_delimiter);
		*params.ret = result;
	}
	else if (params.tok == '>')
		*params.ret = replace_output_redir(*params.ret,
				params.file_or_delimiter);
	else if (params.tok == '+')
		*params.ret = replace_append_redir(*params.ret,
				params.file_or_delimiter);
	else if (params.tok == 'H')
		*params.ret = handle_heredoc_token(*params.ret,
				params.file_or_delimiter, params.env_copy,
				params.was_quoted);
	return (0);
}

static char	*process_file_or_delimiter(t_redir_file_params params,
	int was_quoted)
{
	char	*file_or_delimiter;

	if (params.tok == 'H')
	{
		file_or_delimiter = process_heredoc_delimiter(*params.q, *params.eq,
				params.env_copy, was_quoted);
	}
	else
	{
		file_or_delimiter = process_filename(*params.q, *params.eq,
				params.env_copy);
	}
	return (file_or_delimiter);
}

int	process_redirection_file(t_redir_file_params params)
{
	char				*file_or_delimiter;
	int					was_quoted;
	t_redir_cmd_params	cmd_params;

	was_quoted = check_quoted_status(params.q, params.eq);
	file_or_delimiter = process_file_or_delimiter(params, was_quoted);
	if (!file_or_delimiter)
	{
		free_cmd(*params.ret);
		return (1);
	}
	setup_redirection_cmd_params(&cmd_params, params,
		file_or_delimiter, was_quoted);
	create_redirection_cmd(cmd_params);
	return (0);
}
