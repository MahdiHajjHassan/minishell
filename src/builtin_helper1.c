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

	i = 0;
	while (env_copy && env_copy[i])
	{
		if (ft_strncmp(env_copy[i], "HOME=", 5) == 0)
		{
			home = env_copy[i] + 5;
			if (chdir(home) != 0)
			{
				ft_fprintf_stderr("minishell: cd: %s: %s\n", home,
					strerror(errno));
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
		ft_fprintf_stderr("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}

int	parse_export_arg(char *arg, char **name, char **value)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (! equals)
	{
		ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n",
			arg);
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

void	format_export_output(char *env_var)
{
	char	*equals;
	char	*name;
	char	*value;

	equals = ft_strchr(env_var, '=');
	if (! equals)
	{
		ft_putstr_fd(env_var, STDOUT_FILENO);
		return ;
	}
	*equals = '\0';
	name = env_var;
	value = equals + 1;
	ft_putstr_fd(name, STDOUT_FILENO);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd(value, STDOUT_FILENO);
	ft_putstr_fd("\"", STDOUT_FILENO);
	*equals = '=';
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

static char	**copy_env_to_sorted_array(char **env_copy, int count)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc((count + 1) * sizeof(char *));
	if (! sorted_env)
		return (NULL);
	i = 0;
	while (env_copy && env_copy[i])
	{
		sorted_env[i] = ft_strdup(env_copy[i]);
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}

static void	sort_env_array(char **sorted_env)
{
	int		i;
	int		j;
	char	*temp;

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
}

static void	print_sorted_array(char **sorted_env)
{
	int		i;

	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		format_export_output(sorted_env[i]);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

static void	free_sorted_array(char **sorted_env)
{
	int		i;

	i = 0;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

void	print_sorted_env_vars(char **env_copy)
{
	int		count;
	char	**sorted_env;

	count = 0;
	while (env_copy && env_copy[count])
		count++;
	sorted_env = copy_env_to_sorted_array(env_copy, count);
	if (! sorted_env)
		return ;
	sort_env_array(sorted_env);
	print_sorted_array(sorted_env);
	free_sorted_array(sorted_env);
}

static char	*create_old_pwd_var(char *old_pwd)
{
	char	*old_pwd_var;

	old_pwd_var = malloc(ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	if (! old_pwd_var)
		return (NULL);
	ft_strcpy(old_pwd_var, "OLDPWD=");
	ft_strlcat(old_pwd_var, old_pwd,
		ft_strlen("OLDPWD") + ft_strlen(old_pwd) + 2);
	return (old_pwd_var);
}

static char	*create_new_pwd_var(char *new_pwd)
{
	char	*new_pwd_var;

	new_pwd_var = malloc(ft_strlen("PWD") + ft_strlen(new_pwd) + 2);
	if (! new_pwd_var)
		return (NULL);
	ft_strcpy(new_pwd_var, "PWD=");
	ft_strlcat(new_pwd_var, new_pwd,
		ft_strlen("PWD") + ft_strlen(new_pwd) + 2);
	return (new_pwd_var);
}

static char	**allocate_new_environ(char ***env_copy, int count)
{
	char	**new_environ;

	(void)env_copy;
	new_environ = malloc((count + 3) * sizeof(char *));
	if (! new_environ)
		return (NULL);
	return (new_environ);
}

static void	copy_and_update_env_vars(char ***env_copy, char **new_environ,
		char *old_pwd_var, char *new_pwd_var)
{
	int		i;

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
}

static void	finalize_new_environ(char **new_environ, int count,
		char *old_pwd_var, char *new_pwd_var)
{
	if (count == 0)
	{
		new_environ[0] = old_pwd_var;
		new_environ[1] = new_pwd_var;
		new_environ[2] = NULL;
	}
	else
	{
		new_environ[count] = NULL;
	}
}

static int	count_env_vars(char ***env_copy)
{
	int		count;

	count = 0;
	while ((*env_copy)[count])
		count++;
	return (count);
}

static int	prepare_pwd_variables(t_pwd_prep_params params)
{
	int		count;

	*params.old_pwd_var = create_old_pwd_var(params.old_pwd);
	*params.new_pwd_var = create_new_pwd_var(params.new_pwd);
	if (! *params.old_pwd_var || ! *params.new_pwd_var)
	{
		free(*params.old_pwd_var);
		free(*params.new_pwd_var);
		return (0);
	}
	count = count_env_vars(params.env_copy);
	*params.new_environ = allocate_new_environ(params.env_copy, count);
	if (! *params.new_environ)
	{
		free(*params.old_pwd_var);
		free(*params.new_pwd_var);
		return (0);
	}
	return (count);
}

void	update_pwd_variables(char *old_pwd, char *new_pwd, char ***env_copy)
{
	char	**new_environ;
	char	*old_pwd_var;
	char	*new_pwd_var;
	int		count;

	count = prepare_pwd_variables((t_pwd_prep_params){old_pwd, new_pwd,
			env_copy, &old_pwd_var, &new_pwd_var, &new_environ});
	if (count == 0)
		return ;
	copy_and_update_env_vars(env_copy, new_environ, old_pwd_var, new_pwd_var);
	finalize_new_environ(new_environ, count, old_pwd_var, new_pwd_var);
	free(*env_copy);
	*env_copy = new_environ;
}
