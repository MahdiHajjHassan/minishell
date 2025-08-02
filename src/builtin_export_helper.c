/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
