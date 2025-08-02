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
		ft_fprintf_stderr("minishell: too many arguments\n");
		free(processed);
		return ;
	}
	cmd->av[*argc] = processed;
	cmd->eav[*argc] = cmd->av[*argc] + ft_strlen(processed);
	(*argc)++;
}

void	finalize_exec_cmd(struct s_execcmd *cmd, int argc)
{
	cmd->av[argc] = 0;
	cmd->eav[argc] = 0;
}

static char	*allocate_and_copy_result(char *result, char *temp)
{
	size_t	total_len;
	char	*new_result;

	total_len = ft_strlen(result) + ft_strlen(temp);
	new_result = malloc(total_len + 1);
	if (! new_result)
	{
		free(result);
		free(temp);
		return (NULL);
	}
	ft_strcpy(new_result, result);
	ft_strlcat(new_result, temp, total_len + 1);
	free(result);
	free(temp);
	return (new_result);
}

static char	*process_quoted_segment(char *current_pos, char *input_end,
		char **env_copy, char *result)
{
	char	*q;
	char	*eq;
	char	*temp;
	int		quote_type;

	quote_type = *current_pos;
	q = current_pos + 1;
	eq = q;
	while (eq < input_end && *eq != quote_type)
		eq++;
	if (eq >= input_end)
	{
		free(result);
		return (NULL);
	}
	temp = process_argument_with_expansion(q, eq, env_copy, quote_type);
	if (! temp)
	{
		free(result);
		return (NULL);
	}
	return (allocate_and_copy_result(result, temp));
}

static char	*skip_whitespace_chars(char *current_pos, char *input_end)
{
	while (current_pos < input_end
		&& (*current_pos == ' ' || *current_pos == '\t'))
		current_pos++;
	return (current_pos);
}

static char	*handle_quoted_string(char **current_pos, char *input_end,
		char **env_copy, char *result)
{
	result = process_quoted_segment(*current_pos, input_end, env_copy, result);
	if (! result)
		return (NULL);
	*current_pos = *current_pos + 1;
	while (*current_pos < input_end && **current_pos != '"'
		&& **current_pos != '\'')
		(*current_pos)++;
	if (*current_pos < input_end)
		(*current_pos)++;
	return (result);
}

static char	*initialize_concatenation(char **input_ptr)
{
	char	*result;

	(void)input_ptr;
	result = ft_strdup("");
	if (! result)
		return (NULL);
	return (result);
}

static char	*process_concatenation_loop(char *result, char *current_pos,
		char *input_end, char **env_copy)
{
	while (current_pos < input_end)
	{
		current_pos = skip_whitespace_chars(current_pos, input_end);
		if (current_pos >= input_end)
			break ;
		if (*current_pos == '"' || *current_pos == '\'')
		{
			result = handle_quoted_string(&current_pos, input_end, env_copy,
					result);
			if (! result)
				return (NULL);
		}
		else
		{
			break ;
		}
	}
	return (result);
}

char	*concatenate_quoted_strings(char **input_ptr, char *input_end,
		char **env_copy)
{
	char	*result;
	char	*current_pos;

	result = initialize_concatenation(input_ptr);
	if (! result)
		return (NULL);
	current_pos = *input_ptr;
	result = process_concatenation_loop(result, current_pos, input_end,
			env_copy);
	if (! result)
		return (NULL);
	*input_ptr = current_pos;
	return (result);
}

static int	process_expanded_argument(struct s_cmd *ret,
		t_process_args_params params, char **env_copy, char *q, char *eq)
{
	char	*processed;
	int		quote_type;

	quote_type = remove_exec_quotes(&q, &eq);
	processed = process_argument_with_expansion(q, eq, env_copy,
			quote_type);
	if (! processed)
	{
		free_cmd(ret);
		return (1);
	}
	add_argument(params.cmd, processed, params.argc);
	return (0);
}

static int	process_exec_token(struct s_cmd *ret, t_process_args_params params,
		char **env_copy)
{
	char	*q;
	char	*eq;
	int		tok;

	tok = get_exec_token(params.input_ptr, params.input_end, &q, &eq);
	if (tok == 0)
		return (0);
	return (process_expanded_argument(ret, params, env_copy, q, eq));
}

static int	process_single_exec_argument(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	return (process_exec_token(ret, params, env_copy));
}

struct s_cmd	*process_arguments(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	while (! peek(params.input_ptr, params.input_end, "|)&;"))
	{
		if (process_single_exec_argument(ret, params, env_copy))
			return (NULL);
	}
	return (ret);
}

struct s_cmd	*handle_heredoc_token(struct s_cmd *cmd, char *delimiter,
		char **env_copy, int is_quoted)
{
	char	*content;

	content = read_heredoc_content(delimiter, env_copy, is_quoted);
	if (! content)
	{
		free(delimiter);
		return (NULL);
	}
	return (heredoccmd(cmd, delimiter, content));
}

static int	validate_filename_token(int filename_tok, struct s_cmd **ret,
		t_process_args_params params)
{
	if (filename_tok == '<' || filename_tok == '>'
		|| filename_tok == '+' || filename_tok == 'H')
	{
		*params.input_ptr = params.input_end;
		ft_fprintf_stderr(
			"minishell: syntax error near unexpected token `%c'\n",
			filename_tok);
		free_cmd(*ret);
		return (1);
	}
	ft_fprintf_stderr("minishell: missing file name\n");
	free_cmd(*ret);
	return (1);
}

static int	check_quoted_status(char **q, char **eq)
{
	if (**q == '"' && *(*eq - 1) == '"')
		return (1);
	else if (**q == '\'' && *(*eq - 1) == '\'')
		return (1);
	return (0);
}

static int	create_redirection_cmd(struct s_cmd **ret, char *file_or_delimiter,
		char **env_copy, int tok, int was_quoted)
{
	if (tok == 'H')
	{
		*ret = handle_heredoc_token(*ret, file_or_delimiter,
				env_copy, was_quoted);
	}
	else
	{
		*ret = handle_redir_token(*ret, tok, file_or_delimiter);
	}
	if (! *ret)
		return (1);
	return (0);
}

static int	process_redirection_file(struct s_cmd **ret,
		t_process_args_params params, char **env_copy, int tok, char **q,
		char **eq)
{
	int		was_quoted;
	char	*file_or_delimiter;

	(void)params;
	was_quoted = check_quoted_status(q, eq);
	remove_redir_quotes(q, eq);
	file_or_delimiter = process_filename(*q, *eq, env_copy);
	if (! file_or_delimiter)
	{
		free_cmd(*ret);
		return (1);
	}
	return (create_redirection_cmd(ret, file_or_delimiter, env_copy, tok,
			was_quoted));
}

static int	handle_redirection_token(struct s_cmd **ret,
		t_process_args_params params, char **env_copy, int tok, char **q,
		char **eq)
{
	int		filename_tok;

	filename_tok = gettoken(params.input_ptr, params.input_end, q, eq);
	if (filename_tok != 'a')
		return (validate_filename_token(filename_tok, ret, params));
	return (process_redirection_file(ret, params, env_copy, tok, q, eq));
}

static void	skip_to_quote_end(char **temp_ptr, char *temp_end, char quote_char)
{
	(*temp_ptr)++;
	while (*temp_ptr < temp_end && **temp_ptr != quote_char)
		(*temp_ptr)++;
	if (*temp_ptr < temp_end)
		(*temp_ptr)++;
}

static int	count_consecutive_quotes(char **q, char **eq)
{
	char	*temp_ptr;
	char	*temp_end;
	int		consecutive_quotes;
	char	quote_char;

	temp_ptr = *q;
	temp_end = *eq;
	consecutive_quotes = 0;
	while (temp_ptr < temp_end)
	{
		if (*temp_ptr == '"' || *temp_ptr == '\'')
		{
			quote_char = *temp_ptr;
			skip_to_quote_end(&temp_ptr, temp_end, quote_char);
			consecutive_quotes++;
		}
		else
			break ;
	}
	return (consecutive_quotes);
}

static int	process_consecutive_quotes(struct s_cmd **ret,
		t_process_args_params params, char **env_copy, char **q, char **eq)
{
	char	*concatenated;

	concatenated = concatenate_quoted_strings(q, *eq, env_copy);
	if (! concatenated)
	{
		free_cmd(*ret);
		return (1);
	}
	add_argument(params.cmd, concatenated, params.argc);
	return (0);
}

static int	process_single_argument(struct s_cmd **ret,
		t_process_args_params params, char **env_copy, char **q, char **eq)
{
	char	*processed;
	int		quote_type;

	quote_type = remove_exec_quotes(q, eq);
	processed = process_argument_with_expansion(*q, *eq, env_copy,
			quote_type);
	if (! processed)
	{
		free_cmd(*ret);
		return (1);
	}
	add_argument(params.cmd, processed, params.argc);
	return (0);
}

static int	handle_argument_token(struct s_cmd **ret,
		t_process_args_params params, char **env_copy, char **q, char **eq)
{
	int	consecutive_quotes;

	consecutive_quotes = count_consecutive_quotes(q, eq);
	if (consecutive_quotes > 1)
		return (process_consecutive_quotes(ret, params, env_copy, q, eq));
	else
		return (process_single_argument(ret, params, env_copy, q, eq));
}

static int	handle_token_type(struct s_cmd **ret, t_process_args_params params,
		char **env_copy, int tok, char **q, char **eq)
{
	if (tok == '"' || tok == '\'')
	{
		free_cmd(*ret);
		return (1);
	}
	if (tok == '<' || tok == '>' || tok == '+' || tok == 'H')
	{
		if (handle_redirection_token(ret, params, env_copy, tok, q, eq))
			return (1);
	}
	else if (tok == 'a')
	{
		if (handle_argument_token(ret, params, env_copy, q, eq))
			return (1);
	}
	else
	{
		return (1);
	}
	return (0);
}

struct s_cmd	*process_arguments_and_redirs(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	char	*q;
	char	*eq;
	int		tok;

	while (! peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = gettoken(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		if (handle_token_type(&ret, params, env_copy, tok, &q, &eq))
			return (NULL);
	}
	return (ret);
}
