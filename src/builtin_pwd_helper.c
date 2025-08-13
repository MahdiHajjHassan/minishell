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
	free_environ_copy(*env_copy);
	*env_copy = new_environ;
	free(old_pwd_var);
	free(new_pwd_var);
}

int	builtin_env(char **argv, char ***env_copy)
{
	struct s_execcmd	ex;
	int				j;

	if (argv[1])
	{
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

    (void)argv;
    if (getcwd(cwd, sizeof(cwd)))
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        return (0);
    }
    if (env_copy && *env_copy)
    {
        char *pwd_value = get_env_value("PWD", 3, *env_copy);
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

void	finalize_new_environ_pwd(char **new_environ, int count,
		char *old_pwd_var, char *new_pwd_var)
{
	int		i;
	int		oldpwd_found;
	int		pwd_found;

	(void)old_pwd_var;
	(void)new_pwd_var;
	oldpwd_found = 0;
	pwd_found = 0;
	i = 0;
	while (i < count)
	{
		if (new_environ[i] == old_pwd_var)
			oldpwd_found = 1;
		if (new_environ[i] == new_pwd_var)
			pwd_found = 1;
		i++;
	}
	i = count;
	if (!oldpwd_found)
		i++;
	if (!pwd_found)
		i++;
	new_environ[i] = NULL;
}
