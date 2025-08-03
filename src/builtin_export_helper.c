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

static int	is_valid_identifier(const char *name)
{
	size_t	i;

	if (!name || !*name)
		return (0);
	
	// First character must be a letter or underscore
	if ((*name < 'A' || *name > 'Z') && (*name < 'a' || *name > 'z') && *name != '_')
		return (0);
	
	// Remaining characters must be letters, digits, or underscores
	i = 1;
	while (name[i])
	{
		if ((name[i] < 'A' || name[i] > 'Z') && 
			(name[i] < 'a' || name[i] > 'z') && 
			(name[i] < '0' || name[i] > '9') && 
			name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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
	
	// Validate the identifier name
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
