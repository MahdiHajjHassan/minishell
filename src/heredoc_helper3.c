/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	initialize_heredoc_content(char **stripped_delimiter,
	char **content, char *delimiter)
{
	*stripped_delimiter = ft_strdup(delimiter);
	*content = ft_strdup("");
	if (! *content)
	{
		free(*stripped_delimiter);
		return (0);
	}
	return (1);
}

void	cleanup_heredoc_content(char *stripped_delimiter)
{
	init_signals();
	free(stripped_delimiter);
}
