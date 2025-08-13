/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper_helper1.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_concatenate_next_token(t_arg_process_params arg_params)
{
	char	*next_pos;

	next_pos = *arg_params.eq;
	while (next_pos < arg_params.params.input_end
		&& (*next_pos == ' ' || *next_pos == '\t'))
		next_pos++;
    if (next_pos != *arg_params.eq
        || next_pos >= arg_params.params.input_end
        || *next_pos == '|'
        || *next_pos == '&' || *next_pos == ';' || *next_pos == '<'
        || *next_pos == '>')
		return (0);
	return (1);
}

static int	concatenate_strings(char **result, char *temp)
{
	char	*new_result;

	new_result = malloc(ft_strlen(*result) + ft_strlen(temp) + 1);
	if (!new_result)
	{
		free(*result);
		free(temp);
		return (1);
	}
	ft_strcpy(new_result, *result);
	ft_strlcat(new_result, temp,
		ft_strlen(*result) + ft_strlen(temp) + 1);
	free(*result);
	free(temp);
	*result = new_result;
	return (0);
}

int	process_next_token_for_concatenation(t_arg_process_params arg_params,
	char **result, char **next_q, char **next_eq)
{
	int						next_tok;
	char					*temp;
	t_arg_process_params	next_params;

	next_tok = gettoken(arg_params.params.input_ptr,
			arg_params.params.input_end, next_q, next_eq);
	if (next_tok != 'a' && next_tok != '"' && next_tok != '\'')
		return (0);
	next_params = arg_params;
	next_params.q = next_q;
	next_params.eq = next_eq;
	temp = process_single_argument_internal(next_params);
	if (!temp)
	{
		free(*result);
		return (1);
	}
	return (concatenate_strings(result, temp));
}
