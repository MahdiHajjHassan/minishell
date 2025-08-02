/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = parse_n_flag(argv, &print_newline);
	print_echo_args(argv, i);
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

static int	parse_n_flag(char **argv, int *print_newline)
{
	int	i;
	int	j;

	i = 1;
	*print_newline = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
	{
		j = 2;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
		{
			*print_newline = 0;
			i++;
		}
		else
			break ;
	}
	return (i);
}

static void	print_echo_args(char **argv, int i)
{
	int	first;

	first = 1;
	while (argv[i])
	{
		if (!first)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		first = 0;
		i++;
	}
}

int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
