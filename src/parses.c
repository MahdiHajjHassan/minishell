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
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

static char	*process_escaped(const char *input, size_t len)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = malloc(len + 1);
	i = 0;
	j = 0;
	if (!output)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
		{
			switch (input[i + 1])
			{
				case 'n': output[j++] = '\n'; break;
				case 't': output[j++] = '\t'; break;
				case 'r': output[j++] = '\r'; break;
				case 'v': output[j++] = '\v'; break;
				case 'b': output[j++] = '\b'; break;
				case 'f': output[j++] = '\f'; break;
				case 'a': output[j++] = '\a'; break;
				case '\\': output[j++] = '\\'; break;
				default: output[j++] = input[i + 1]; break;
			}
			i += 2;
		}
		else
		{
			output[j++] = input[i++];
		}
	}
	output[j] = '\0';
	return (output);
}

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

struct s_cmd	*parse_redirs(struct s_cmd *cmd, char **input_ptr, char *input_end)
{
	int		tok;
	char	*q;
	char	*eq;
	char	*file;
	char	*processed;
	char	*expanded;
	size_t	len;

	while (peek(input_ptr, input_end, "<>"))
	{
		tok = gettoken(input_ptr, input_end, NULL, NULL);
		if (gettoken(input_ptr, input_end, &q, &eq) != 'a')
		{
			fprintf(stderr, "missing file name\n");
			wtf();
		}
		if (*q == '"' && *(eq - 1) == '"')
		{
			q++;
			eq--;
		}
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
		file = expanded;
		switch (tok)
		{
			case '<':
				cmd = redircmd(cmd, file, file + strlen(file), O_RDONLY, 0);
				break;
			case '>':
				cmd = redircmd(cmd, file, file + strlen(file), 
					O_WRONLY | O_CREAT | O_TRUNC | 0644, 1);
				break;
			case '+':
				cmd = redircmd(cmd, file, file + strlen(file),
					O_WRONLY | O_CREAT | O_APPEND | 0644, 1);
				break;
		}
	}
	return (cmd);
}

struct s_cmd	*parseexec(char **input_ptr, char *input_end)
{
	struct s_execcmd	*cmd;
	char			*q;
	char			*eq;
	struct s_cmd		*ret;
	int				argc;
	int				tok;
	size_t			len;
	char			*processed;

	argc = 0;
	if (peek(input_ptr, input_end, "("))
		return (parse_block(input_ptr, input_end));
	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	ret = parse_redirs(ret, input_ptr, input_end);
	while (!peek(input_ptr, input_end, "|)&;"))
	{
		if ((tok = gettoken(input_ptr, input_end, &q, &eq)) == 0)
			break;
		if (tok != 'a')
		{
			fprintf(stderr, "missing file name\n");
			wtf();
		}
		if (*q == '"' && *(eq - 1) == '"')
		{
			q++;
			eq--;
		}
		else if (*q == '\'' && *(eq - 1) == '\'')
		{
			q++;
			eq--;
		}
		len = eq - q;
		processed = process_escaped(q, len);
		if (!processed)
		{
			fprintf(stderr, "malloc failed\n");
			wtf();
		}
		cmd->av[argc] = processed;
		cmd->eav[argc] = cmd->av[argc] + strlen(processed);
		argc++;
		if (argc >= MAXARGS)
		{
			fprintf(stderr, "too many args\n");
			wtf();
		}
		ret = parse_redirs(ret, input_ptr, input_end);
	}
	cmd->av[argc] = 0;
	cmd->eav[argc] = 0;
	return (ret);
}
