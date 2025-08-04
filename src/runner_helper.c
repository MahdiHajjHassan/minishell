/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper.c                                     :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_processing_error(char *processed)
{
	if (processed)
		free(processed);
	print_malloc_failed();
	return (NULL);
}

void	print_command_error(const char *cmd, const char *error_msg,
		int exit_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putstr_fd((char *)error_msg, STDERR_FILENO);
	set_exit_status(exit_code);
}

void	handle_absolute_path_error(const char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
		print_command_error(cmd, ": No such file or directory\n", 127);
	else
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			print_command_error(cmd, ": Is a directory\n", 126);
		else
			print_command_error(cmd, ": Permission denied\n", 126);
	}
}

void	run_list_cmd(struct s_cmd *cmd)
{
	(void)cmd;
}
