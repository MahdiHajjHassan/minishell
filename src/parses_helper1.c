/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_output_buffer(size_t len)
{
	char	*output;

	output = malloc(len + 1);
	if (!output)
		return (NULL);
	return (output);
}

char	get_escape_char(char c)
{
	if (c == 'n')
		return ('\n');
	else if (c == 't')
		return ('\t');
	else if (c == 'r')
		return ('\r');
	else if (c == 'v')
		return ('\v');
	else if (c == 'b')
		return ('\b');
	else if (c == 'f')
		return ('\f');
	else if (c == 'a')
		return ('\a');
	else if (c == '\\')
		return ('\\');
	else
		return (c);
}

void	handle_escape_sequence(const char *input, size_t *i,
				char *output, size_t *j)
{
	output[(*j)++] = get_escape_char(input[*i + 1]);
	*i += 2;
}

char	*process_escaped(const char *input, size_t len)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = init_output_buffer(len);
	i = 0;
	j = 0;
	if (!output)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
		{
			handle_escape_sequence(input, &i, output, &j);
		}
		else
		{
			output[j++] = input[i++];
		}
	}
	output[j] = '\0';
	return (output);
}

int	get_redir_token(char **input_ptr, char *input_end,
			char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, NULL, NULL);
	if (gettoken(input_ptr, input_end, q, eq) != 'a')
	{
		fprintf(stderr, "missing file name\n");
		wtf();
	}
	return (tok);
}

void	remove_redir_quotes(char **q, char **eq)
{
	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
	}
}

char	*process_filename(char *q, char *eq)
{
	size_t	len;
	char	*processed;
	char	*expanded;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
	{
		fprintf(stderr, "malloc failed\n");
		wtf();
	}
	expanded = expand_variables(processed, strlen(processed));
	free(processed);
	if (!expanded)
	{
		fprintf(stderr, "malloc failed\n");
		wtf();
	}
	return (expanded);
}

struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + strlen(file),
			(t_redir_params){O_RDONLY, 0}));
}

struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + strlen(file),
			(t_redir_params){O_WRONLY | O_CREAT | O_TRUNC | 0644, 1}));
}

struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + strlen(file),
			(t_redir_params){O_WRONLY | O_CREAT | O_APPEND | 0644, 1}));
}

struct s_cmd	*handle_redir_token(struct s_cmd *cmd, int tok, char *file)
{
	if (tok == '<')
		return (apply_input_redir(cmd, file));
	else if (tok == '>')
		return (apply_output_redir(cmd, file));
	else if (tok == '+')
		return (apply_append_redir(cmd, file));
	return (cmd);
}

struct s_cmd	*init_exec_cmd(void)
{
	struct s_cmd	*ret;

	ret = execcmd();
	return (ret);
}

int	get_exec_token(char **input_ptr, char *input_end, char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, q, eq);
	if (tok == 0)
		return (0);
	if (tok != 'a')
	{
		fprintf(stderr, "missing file name\n");
		wtf();
	}
	return (tok);
}

void	remove_exec_quotes(char **q, char **eq)
{
	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
	}
	else if (**q == '\'' && *(*eq - 1) == '\'')
	{
		(*q)++;
		(*eq)--;
	}
}

char	*process_argument(char *q, char *eq)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	processed = process_escaped(q, len);
	if (!processed)
	{
		fprintf(stderr, "malloc failed\n");
		wtf();
	}
	return (processed);
}

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