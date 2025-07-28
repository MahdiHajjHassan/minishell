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

int	cd_to_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", home, strerror(errno));
		return (1);
	}
	return (0);
}

int	cd_to_path(char *path)
{
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n",
			path, strerror(errno));
		return (1);
	}
	return (0);
}

int	parse_export_arg(char *arg, char **name, char **value)
{
	char	*equals;

	equals = strchr(arg, '=');
	if (!equals)
	{
		fprintf(stderr, "minishell: export: invalid format: %s\n", arg);
		return (1);
	}
	*name = arg;
	*equals = '\0';
	*value = equals + 1;
	return (0);
}

void	remove_quotes(char **value)
{
	if (**value == '"' && (*value)[strlen(*value) - 1] == '"')
	{
		(*value)++;
		(*value)[strlen(*value) - 1] = '\0';
	}
	else if (**value == '\'' && (*value)[strlen(*value) - 1] == '\'')
	{
		(*value)++;
		(*value)[strlen(*value) - 1] = '\0';
	}
}

int	set_environment_var(char *name, char *value)
{
	if (setenv(name, value, 1) != 0)
	{
		fprintf(stderr, "minishell: export: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}
