/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + strlen(file),
			(t_redir_params){O_WRONLY | O_CREAT | O_APPEND | 0644, 1}));
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
