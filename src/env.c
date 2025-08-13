/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_exit_status = 0;

void	set_exit_status(int status)
{
	g_exit_status = status;
}

int	get_exit_status(void)
{
	return (g_exit_status);
}

static char	*finalize_result(char *result, size_t j)
{
	char	*final;

    result[j] = '\0';
    final = malloc(j + 1);
    if (!final)
        return (result);
    ft_memcpy(final, result, j + 1);
    free(result);
    return (final);
}

char	*expand_variables(const char *str, size_t len, char **env_copy)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	alloc_size;

	if (! str)
		return (ft_strdup(""));
	i = 0;
	j = 0;
	result = init_result_buffer(len, &alloc_size);
	if (! result)
		return (NULL);
	while (i < len)
	{
		if (process_character((t_process_char_params){str, len, &i,
				&result, &j, &alloc_size, env_copy}))
			return (NULL);
	}
	return (finalize_result(result, j));
}
