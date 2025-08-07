/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_too_many_args(void)
{
	ft_putstr_fd("minishell: too many arguments\n", STDERR_FILENO);
}

void	print_missing_paren(const char *paren)
{
	ft_putstr_fd("minishell: missing ", STDERR_FILENO);
	ft_putstr_fd((char *)paren, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_unknown_command_type(int type)
{
	char	*type_str;

	type_str = ft_itoa(type);
	ft_putstr_fd("unknown command type: ", STDERR_FILENO);
	if (type_str)
	{
		ft_putstr_fd(type_str, STDERR_FILENO);
		free(type_str);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_heredoc_eof_warning(const char *delimiter)
{
	ft_putstr_fd(
		"minishell: warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO);
	ft_putstr_fd((char *)delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

int	ft_fprintf_stderr(const char *format, ...)
{
	if (ft_strcmp(format, "minishell: cd: HOME not set\n") == 0)
		print_cd_home_not_set();
	else if (ft_strcmp(format, "minishell: cd: getcwd failed\n") == 0)
		print_cd_getcwd_failed();
	else if (ft_strcmp(format, "minishell: cd: too many arguments\n") == 0)
		print_cd_too_many_args();
	else if (ft_strcmp(format, "minishell: syntax error\n") == 0)
		print_syntax_error();
	else if (ft_strcmp(format, "minishell: missing file name\n") == 0)
		print_missing_file_name();
	else if (ft_strcmp(format, "minishell: malloc failed\n") == 0)
		print_malloc_failed();
	else if (ft_strcmp(format, "minishell: too many arguments\n") == 0)
		print_too_many_args();
	else if (ft_strcmp(format, "minishell: missing (\n") == 0)
		print_missing_paren("(");
	else if (ft_strcmp(format, "minishell: missing )\n") == 0)
		print_missing_paren(")");
	else if (ft_strcmp(format, "minishell: exit: too many arguments\n") == 0)
		print_exit_too_many_args();
	else
		ft_putstr_fd((char *)format, STDERR_FILENO);
	return (0);
}
