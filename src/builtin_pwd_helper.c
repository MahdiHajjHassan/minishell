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
	free_environ_copy(*env_copy);
	*env_copy = new_environ;
	free(old_pwd_var);
	free(new_pwd_var);
}

static int	handle_env_with_args(char **argv, char ***env_copy)
{
	struct s_execcmd	ex;
	int					j;

	ft_bzero(&ex, sizeof(ex));
	ex.type = EXEC;
	j = 0;
	while (argv[1 + j] && j < MAXARGS - 1)
	{
		ex.av[j] = argv[1 + j];
		j++;
	}
	ex.av[j] = NULL;
	if (is_builtin(ex.av[0]))
		handle_exec_builtin(&ex, (struct s_cmd *)&ex, env_copy);
	else
		execute_external_cmd(&ex, *env_copy);
	return (get_exit_status());
}

int	builtin_env(char **argv, char ***env_copy)
{
	int	j;

	if (argv[1])
		return (handle_env_with_args(argv, env_copy));
	j = 0;
	while ((*env_copy)[j])
	{
		ft_putstr_fd((*env_copy)[j], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		j++;
	}
	return (0);
}

int	builtin_pwd(char **argv, char ***env_copy)
{
	char	cwd[1024];
	char	*pwd_value;

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	if (env_copy && *env_copy)
	{
		pwd_value = get_env_value("PWD", 3, *env_copy);
		if (pwd_value && *pwd_value)
		{
			ft_putstr_fd(pwd_value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			free(pwd_value);
			return (0);
		}
		free(pwd_value);
	}
	perror("pwd");
	return (1);
}
