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

int	builtin_exit(char **argv)
{
	int	status;

	status = 0;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argv[1])
	{
		if (!check_numeric_argument(argv[1]))
		{
			print_exit_error(argv[1]);
			clean_exit(255);
		}
		status = ft_atoi(argv[1]);
		if (argv[2])
		{
			ft_fprintf_stderr("minishell: exit: too many arguments\n",
				 STDERR_FILENO);
			return (1);
		}
	}
	clean_exit(status);
	return (0);
}

static int	check_numeric_argument(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	print_exit_error(char *arg)
{
	ft_fprintf_stderr("minishell: exit: ", STDERR_FILENO);
	ft_fprintf_stderr(arg, STDERR_FILENO);
	ft_fprintf_stderr(": numeric argument required\n", STDERR_FILENO);
}
