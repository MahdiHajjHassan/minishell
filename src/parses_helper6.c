/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exec_token(char **input_ptr, char *input_end, char **q, char **eq)
{
	int	tok;

	tok = gettoken(input_ptr, input_end, q, eq);
	if (tok == 0)
		return (0);
	if (tok != 'a')
	{
		ft_fprintf_stderr("minishell: missing file name\n");
		return (0);
	}
	return (tok);
}

int	remove_exec_quotes(char **q, char **eq)
{
	int	quote_type;

	quote_type = 0;
	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
		quote_type = '"';
	}
	else if (**q == '\'' && *(*eq - 1) == '\'')
	{
		(*q)++;
		(*eq)--;
		quote_type = '\'';
	}
	return (quote_type);
}

char	*process_argument(char *q, char *eq)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	processed = process_escaped(q, len);
	if (! processed)
	{
		ft_fprintf_stderr("minishell: malloc failed\n");
		return (NULL);
	}
	return (processed);
}

char	*process_argument_with_expansion(char *q, char *eq, char **env_copy,
		int quote_type)
{
	size_t	len;
	char	*processed;
	char	*expanded;

	len = eq - q;
	processed = process_escaped(q, len);
	if (! processed)
	{
		ft_fprintf_stderr("minishell: malloc failed\n");
		return (NULL);
	}
	if (quote_type != '\'')
	{
		expanded = expand_variables(processed, ft_strlen(processed), env_copy);
		free(processed);
		if (! expanded)
		{
			ft_fprintf_stderr("minishell: malloc failed\n");
			return (NULL);
		}
		return (expanded);
	}
	return (processed);
}
