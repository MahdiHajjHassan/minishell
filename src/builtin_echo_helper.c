/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	j = 2;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

int	process_n_flags(char **argv, int *i)
{
	int	print_newline;

	print_newline = 1;
	while (argv[*i] && check_n_flag(argv[*i]))
	{
		print_newline = 0;
		(*i)++;
	}
	return (print_newline);
}

void	print_arguments(char **argv, int start_index)
{
	int	i;

	i = start_index;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = process_n_flags(argv, &i);
	print_arguments(argv, i);
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
