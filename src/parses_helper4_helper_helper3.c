/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper_helper3.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_argument_logic(char *processed, int quote_type,
	t_arg_process_params arg_params)
{
	int	should_split;

	should_split = (quote_type == 0
			&& !is_assignment_word_local(processed)
			&& (ft_strchr(processed, ' ') || ft_strchr(processed, '\t')));
	if (should_split)
	{
		if (split_and_add_arguments(processed, arg_params))
			return (1);
		free(processed);
	}
	else
	{
		add_argument(arg_params.params.cmd, processed, arg_params.params.argc);
	}
	return (0);
}
