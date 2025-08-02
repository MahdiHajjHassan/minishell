/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdarg.h>

int	add_new_var(char ***env_copy, const char *name,
	const char *value)
{
	char	*new_var;
	char	**new_environ;
	int		count;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (! new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	count = allocate_new_environ(env_copy, &new_var, &new_environ);
	if (count == -1)
		return (-1);
	copy_existing_env_vars(env_copy, new_environ, count);
	finalize_new_environ(new_environ, count, new_var, env_copy);
	return (0);
}

int	ft_fprintf_stderr(const char *format, ...)
{
	va_list	args;
	char	buffer[1024];
	int		len;

	va_start(args, format);
	len = ft_vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	if (len > 0)
		write(STDERR_FILENO, buffer, len);
	return (len);
}
