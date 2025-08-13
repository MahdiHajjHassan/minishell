/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management_helper3.c                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*setup_pwd_variable(void)
{
	char		cwd_buf[PATH_MAX];
	char		*pwd_var;

	if (getcwd(cwd_buf, sizeof(cwd_buf)) != NULL)
	{
		pwd_var = malloc(ft_strlen("PWD=") + ft_strlen(cwd_buf) + 1);
		if (!pwd_var)
			return (NULL);
		ft_strcpy(pwd_var, "PWD=");
		ft_strlcat(pwd_var, cwd_buf,
			ft_strlen("PWD=") + ft_strlen(cwd_buf) + 1);
	}
	else
	{
		pwd_var = ft_strdup("PWD=/");
		if (!pwd_var)
			return (NULL);
	}
	return (pwd_var);
}

static char	*setup_path_variable(void)
{
	const char	*default_path
		= "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	char		*path_var;

	path_var = malloc(ft_strlen("PATH=") + ft_strlen(default_path) + 1);
	if (!path_var)
		return (NULL);
	ft_strcpy(path_var, "PATH=");
	ft_strlcat(path_var, default_path,
		ft_strlen("PATH=") + ft_strlen(default_path) + 1);
	return (path_var);
}

static char	*setup_shlvl_variable(void)
{
	char	*shlvl_var;

	shlvl_var = malloc(ft_strlen("SHLVL=1") + 1);
	if (!shlvl_var)
		return (NULL);
	ft_strcpy(shlvl_var, "SHLVL=1");
	return (shlvl_var);
}

static void	cleanup_environ_on_error(char **new_environ, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(new_environ[i]);
		i++;
	}
	free(new_environ);
}

char	**create_default_environ(void)
{
	char	**new_environ;

	new_environ = malloc(4 * sizeof(char *));
	if (!new_environ)
		return (NULL);
	new_environ[0] = setup_shlvl_variable();
	if (!new_environ[0])
	{
		free(new_environ);
		return (NULL);
	}
	new_environ[1] = setup_path_variable();
	if (!new_environ[1])
	{
		cleanup_environ_on_error(new_environ, 1);
		return (NULL);
	}
	new_environ[2] = setup_pwd_variable();
	if (!new_environ[2])
	{
		cleanup_environ_on_error(new_environ, 2);
		return (NULL);
	}
	new_environ[3] = NULL;
	return (new_environ);
}
