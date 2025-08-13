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
		print_export_invalid_identifier(arg);
		return (1);
	}
	*name = arg;
	*equals = '\0';
	*value = equals + 1;
	if (!is_valid_identifier(*name))
	{
		print_export_invalid_identifier(arg);
		return (1);
	}
	return (0);
}

void	remove_quotes(char **value)
{
	size_t	len;

	if (!*value || !**value)
		return ;
	len = ft_strlen(*value);
	if (len < 2)
		return ;
	if (**value == '"' && (*value)[len - 1] == '"')
	{
		(*value)++;
		(*value)[len - 2] = '\0';
	}
	else if (**value == '\'' && (*value)[len - 1] == '\'')
	{
		(*value)++;
		(*value)[len - 2] = '\0';
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
		print_export_error(strerror(errno));
		return (1);
	}
	return (0);
}

int	handle_export_no_equals(char *arg_copy, char ***env_copy)
{
	if (!is_valid_identifier(arg_copy))
	{
		print_export_invalid_identifier(arg_copy);
		free(arg_copy);
		return (1);
	}
	if (set_environment_var(arg_copy, "", env_copy))
	{
		free(arg_copy);
		return (1);
	}
	free(arg_copy);
	return (0);
}

int	handle_export_with_equals(char *arg_copy, char ***env_copy)
{
	char	*name;
	char	*value;

	if (process_export_arg(arg_copy, &name, &value, env_copy))
		return (1);
	return (0);
}
