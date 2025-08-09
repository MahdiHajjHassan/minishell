/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_single_argument_internal(t_arg_process_params arg_params)
{
	char	*processed;
	int		quote_type;
	char	*q_copy;
	char	*eq_copy;

	q_copy = *arg_params.q;
	eq_copy = *arg_params.eq;
	quote_type = remove_exec_quotes(&q_copy, &eq_copy);
	processed = process_argument_with_expansion(q_copy,
			eq_copy, arg_params.env_copy, quote_type);
	return (processed);
}

int	concatenate_adjacent_tokens(t_arg_process_params arg_params)
{
	char	*result;
	char	*next_q;
	char	*next_eq;

	result = process_single_argument_internal(arg_params);
	if (! result)
		return (1);
	while (1)
	{
		if (! should_concatenate_next_token(arg_params))
			break ;
		if (process_next_token_for_concatenation(arg_params, &result,
				&next_q, &next_eq))
			return (1);
		*arg_params.eq = next_eq;
	}
	add_argument(arg_params.params.cmd, result, arg_params.params.argc);
	return (0);
}
