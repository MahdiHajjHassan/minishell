/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_home(char **env_copy)
{
	char	*home;
	int		i;

	/* Search for HOME in local environment copy */
	i = 0;
	while (env_copy && env_copy[i])
	{
		if (ft_strncmp(env_copy[i], "HOME=", 5) == 0)
		{
			home = env_copy[i] + 5;
			if (chdir(home) != 0)
			{
				ft_fprintf_stderr("minishell: cd: %s: %s\n", home, strerror(errno));
				return (1);
			}
			return (0);
		}
		i++;
	}
	
	ft_fprintf_stderr("minishell: cd: HOME not set\n");
	return (1);
}

int	cd_to_path(char *path)
{
	if (chdir(path) != 0)
	{
		ft_fprintf_stderr("minishell: cd: %s: %s\n",
			path, strerror(errno));
		return (1);
	}
	return (0);
}

int	parse_export_arg(char *arg, char **name, char **value)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	*name = arg;
	*equals = '\0';
	*value = equals + 1;
	return (0);
}

void	remove_quotes(char **value)
{
	if (**value == '"' && (*value)[ft_strlen(*value) - 1] == '"')
	{
		(*value)++;
		(*value)[ft_strlen(*value) - 1] = '\0';
	}
	else if (**value == '\'' && (*value)[ft_strlen(*value) - 1] == '\'')
	{
		(*value)++;
		(*value)[ft_strlen(*value) - 1] = '\0';
	}
}

int	set_environment_var(char *name, char *value, char ***env_copy)
{
	if (ft_setenv(name, value, 1, env_copy) != 0)
	{
		ft_fprintf_stderr("minishell: export: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

void	print_sorted_env_vars(char **env_copy)
{
	int		i;
	int		j;
	char	*temp;
	int		count;

	/* Count environment variables */
	count = 0;
	while (env_copy && env_copy[count])
		count++;

	/* Create a copy for sorting */
	char **sorted_env = malloc((count + 1) * sizeof(char *));
	if (!sorted_env)
		return;

	/* Copy environment variables */
	i = 0;
	while (env_copy && env_copy[i])
	{
		sorted_env[i] = ft_strdup(env_copy[i]);
		i++;
	}
	sorted_env[i] = NULL;

	/* Sort environment variables */
	i = 0;
	while (sorted_env[i])
	{
		j = i + 1;
		while (sorted_env[j])
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				temp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = temp;
			}
			j++;
		}
		i++;
	}

	/* Print sorted environment variables */
	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(sorted_env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}

	/* Free sorted environment copy */
	i = 0;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

void	update_pwd_variables(char *old_pwd, char *new_pwd, char ***env_copy)
{
	int		i;
	int		count;
	char	**new_environ;
	char	*old_pwd_var;
	char	*new_pwd_var;

	/* Create the variable strings */
	old_pwd_var = malloc(ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	new_pwd_var = malloc(ft_strlen("PWD") + ft_strlen(new_pwd) + 2);
	if (!old_pwd_var || !new_pwd_var)
	{
		free(old_pwd_var);
		free(new_pwd_var);
		return;
	}

	ft_strcpy(old_pwd_var, "OLDPWD=");
	ft_strlcat(old_pwd_var, old_pwd, ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	ft_strcpy(new_pwd_var, "PWD=");
	ft_strlcat(new_pwd_var, new_pwd, ft_strlen("PWD") + ft_strlen(new_pwd) + 2);

	/* Count existing variables */
	count = 0;
	while ((*env_copy)[count])
		count++;

	/* Allocate new array with space for both variables */
	new_environ = malloc((count + 3) * sizeof(char *));
	if (!new_environ)
	{
		free(old_pwd_var);
		free(new_pwd_var);
		return;
	}

	/* Copy existing variables, updating PWD and OLDPWD if they exist */
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], "OLDPWD=", 7) == 0)
		{
			free((*env_copy)[i]);
			new_environ[i] = old_pwd_var;
		}
		else if (ft_strncmp((*env_copy)[i], "PWD=", 4) == 0)
		{
			free((*env_copy)[i]);
			new_environ[i] = new_pwd_var;
		}
		else
		{
			new_environ[i] = (*env_copy)[i];
		}
		i++;
	}

	/* Add new variables if they didn't exist */
	if (i == count) /* No existing PWD or OLDPWD found */
	{
		new_environ[i] = old_pwd_var;
		new_environ[i + 1] = new_pwd_var;
		new_environ[i + 2] = NULL;
	}
	else
	{
		new_environ[i] = NULL;
	}

	/* Update environment pointer */
	free(*env_copy);
	*env_copy = new_environ;
}
