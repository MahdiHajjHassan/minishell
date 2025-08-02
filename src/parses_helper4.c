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

struct s_cmd	*process_arguments_and_redirs(struct s_cmd *ret,
		t_process_args_params params, char **env_copy)
{
	char				*q;
	char				*eq;
	int					tok;
	t_token_type_params	token_params;

	while (! peek(params.input_ptr, params.input_end, "|)&;"))
	{
		tok = gettoken(params.input_ptr, params.input_end, &q, &eq);
		if (tok == 0)
			break ;
		token_params.ret = &ret;
		token_params.params = params;
		token_params.env_copy = env_copy;
		token_params.tok = tok;
		token_params.q = &q;
		token_params.eq = &eq;
		if (handle_token_type(token_params))
			return (NULL);
	}
	return (ret);
}
