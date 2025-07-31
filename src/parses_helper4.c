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
		return;
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

struct s_cmd	*process_arguments(struct s_cmd *ret,
			t_process_args_params params, char **env_copy)
{
	char	*q;
	char	*eq;
	int		tok;
	char	*processed;

	(void)env_copy; /* Parameter not used in this function */
	while (!peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = get_exec_token(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		remove_exec_quotes(&q, &eq);
		processed = process_argument(q, eq);
		add_argument(params.cmd, processed, params.argc);
	}
	return (ret);
}

struct s_cmd	*handle_heredoc_token(struct s_cmd *cmd, char *delimiter)
{
	char	*content;
	
	content = read_heredoc_content(delimiter);
	if (!content)
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

	while (!peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = gettoken(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		
		/* Check if this is a redirection token */
		if (tok == '<' || tok == '>' || tok == '+' || tok == 'H')
		{
			/* Get the filename/delimiter for the redirection */
			int filename_tok = gettoken(params.input_ptr, params.input_end, &q, &eq);
			if (filename_tok != 'a')
			{
				ft_fprintf_stderr("minishell: missing file name\n");
				free_cmd(ret);
				return (NULL);
			}
			
			/* Process the redirection */
			remove_redir_quotes(&q, &eq);
			file_or_delimiter = process_filename(q, eq, env_copy);
			if (!file_or_delimiter)
			{
				free_cmd(ret);
				return (NULL);
			}
			
			if (tok == 'H')
			{
				ret = handle_heredoc_token(ret, file_or_delimiter);
			}
			else
			{
				ret = handle_redir_token(ret, tok, file_or_delimiter);
			}
			
			if (!ret)
				return (NULL);
		}
		else if (tok == 'a')
		{
			/* This is a regular argument */
			remove_exec_quotes(&q, &eq);
			processed = process_argument(q, eq);
			if (!processed)
			{
				free_cmd(ret);
				return (NULL);
			}
			add_argument(params.cmd, processed, params.argc);
		}
		else
		{
			/* Unexpected token */
			break ;
		}
	}
	return (ret);
}
