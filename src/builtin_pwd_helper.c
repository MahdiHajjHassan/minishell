/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_pwd_variables(t_pwd_prep_params params)
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
	count = count_env_vars_pwd(params.env_copy);
	*params.new_environ = allocate_new_environ_pwd(params.env_copy, count);
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
	finalize_new_environ_pwd(new_environ, count, old_pwd_var, new_pwd_var);
	*env_copy = new_environ;
}

int	builtin_env(char **argv, char ***env_copy)
{
	int			i;

	(void)argv;
	i = 0;
	while ((*env_copy)[i])
	{
		ft_putstr_fd((*env_copy)[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (! getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

void	finalize_new_environ_pwd(char **new_environ, int count,
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
