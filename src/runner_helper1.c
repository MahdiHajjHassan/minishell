/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_path(const char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		// First check if file exists
		if (access(cmd, F_OK) != 0)
		{
			// File doesn't exist - this will be handled as "command not found"
			return (NULL);
		}
		// Check if it's a directory
		struct stat st;
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			// It's a directory - this will be handled as "Is a directory"
			return (NULL);
		}
		// File exists and is not a directory, now check if it's executable
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		// File exists but not executable - this will be handled as "permission denied"
		return (NULL);
	}
	return ((char *)1);
}

char	*search_in_paths(char *path, const char *cmd)
{
	char	*curr;
	char	*next;
	char	full_path[1024];
	size_t	len;

	curr = path;
	while (curr && *curr)
	{
		len = get_path_segment_len(curr, &next);
		if (build_full_path(full_path, curr, len, cmd))
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		if (!next)
			break ;
		curr = next + 1;
	}
	return (NULL);
}

void	expand_exec_args(struct s_execcmd *ex, char **env_copy)
{
	int		i;
	char	*original;

	i = 0;
	while (ex->av[i])
	{
		original = ex->av[i];
		ex->av[i] = expand_variables(original, ft_strlen(original), env_copy);
		free(original);
		i++;
	}
}
