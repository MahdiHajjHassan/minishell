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

char	*concatenate_quoted_strings(char **input_ptr, char *input_end,
		char **env_copy)
{
	char	*q;
	char	*eq;
	char	*result;
	char	*temp;
	int		quote_type;
	size_t	total_len;
	char	*current_pos;
	char	*new_result;

	result = ft_strdup("");
	if (! result)
		return (NULL);
	current_pos = *input_ptr;
	while (current_pos < input_end)
	{
		while (current_pos < input_end
			&& (*current_pos == ' ' || *current_pos == '\t'))
			current_pos++;
		if (current_pos >= input_end)
			break ;
		if (*current_pos == '"' || *current_pos == '\'')
		{
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
			temp = process_argument_with_expansion(q, eq, env_copy,
					quote_type);
			if (! temp)
			{
				free(result);
				return (NULL);
			}
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
			result = new_result;
			current_pos = eq + 1;
		}
		else
		{
			break ;
		}
	}
	*input_ptr = current_pos;
	return (result);
}

struct s_cmd	*process_arguments(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	char	*q;
	char	*eq;
	int		tok;
	char	*processed;
	int		quote_type;

	while (! peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = get_exec_token(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		quote_type = remove_exec_quotes(&q, &eq);
		processed = process_argument_with_expansion(q, eq, env_copy,
				quote_type);
		if (! processed)
		{
			free_cmd(ret);
			return (NULL);
		}
		add_argument(params.cmd, processed, params.argc);
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

struct s_cmd	*process_arguments_and_redirs(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	char	*q;
	char	*eq;
	int		tok;
	char	*processed;
	char	*file_or_delimiter;
	int		quote_type;
	char	*concatenated;
	int		filename_tok;
	int		was_quoted;
	char	*temp_ptr;
	char	*temp_end;
	int		consecutive_quotes;
	char	quote_char;

	while (! peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = gettoken(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		if (tok == '"' || tok == '\'')
		{
			free_cmd(ret);
			return (NULL);
		}
		if (tok == '<' || tok == '>' || tok == '+' || tok == 'H')
		{
			filename_tok = gettoken(params.input_ptr, params.input_end, &q,
					&eq);
			if (filename_tok != 'a')
			{
				if (filename_tok == '<' || filename_tok == '>'
					|| filename_tok == '+' || filename_tok == 'H')
				{
					*params.input_ptr = params.input_end;
					ft_fprintf_stderr("minishell: syntax error near unexpected token `%c'\n",
						filename_tok);
					free_cmd(ret);
					return (NULL);
				}
				ft_fprintf_stderr("minishell: missing file name\n");
				free_cmd(ret);
				return (NULL);
			}
			was_quoted = 0;
			if (*q == '"' && *(eq - 1) == '"')
				was_quoted = 1;
			else if (*q == '\'' && *(eq - 1) == '\'')
				was_quoted = 1;
			remove_redir_quotes(&q, &eq);
			file_or_delimiter = process_filename(q, eq, env_copy);
			if (! file_or_delimiter)
			{
				free_cmd(ret);
				return (NULL);
			}
			if (tok == 'H')
			{
				ret = handle_heredoc_token(ret, file_or_delimiter,
						env_copy, was_quoted);
			}
			else
			{
				ret = handle_redir_token(ret, tok, file_or_delimiter);
			}
			if (! ret)
				return (NULL);
		}
		else if (tok == 'a')
		{
			temp_ptr = q;
			temp_end = eq;
			consecutive_quotes = 0;
			while (temp_ptr < temp_end)
			{
				if (*temp_ptr == '"' || *temp_ptr == '\'')
				{
					quote_char = *temp_ptr;
					temp_ptr++;
					while (temp_ptr < temp_end && *temp_ptr != quote_char)
						temp_ptr++;
					if (temp_ptr < temp_end)
					{
						temp_ptr++;
						consecutive_quotes++;
					}
				}
				else
					break ;
			}
			if (consecutive_quotes > 1)
			{
				concatenated = concatenate_quoted_strings(&q, eq,
						env_copy);
				if (! concatenated)
				{
					free_cmd(ret);
					return (NULL);
				}
				add_argument(params.cmd, concatenated, params.argc);
			}
			else
			{
				quote_type = remove_exec_quotes(&q, &eq);
				processed = process_argument_with_expansion(q, eq,
						env_copy, quote_type);
				if (! processed)
				{
					free_cmd(ret);
					return (NULL);
				}
				add_argument(params.cmd, processed, params.argc);
			}
		}
		else
		{
			break ;
		}
	}
	return (ret);
}
