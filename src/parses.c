/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (0);
	ecmd = (struct s_execcmd *)cmd;
	return (!ecmd->av[0] || ft_strlen(ecmd->av[0]) == 0);
}

struct s_cmd	*parse_pipe(char **input_ptr, char *input_end, char **env_copy)
{
	struct s_cmd	*cmd;
	struct s_cmd	*right;

	cmd = parseexec(input_ptr, input_end, env_copy);
	if (! cmd)
		return (NULL);
	if (peek(input_ptr, input_end, "|"))
	{
		gettoken(input_ptr, input_end, NULL, NULL);
		right = parse_pipe(input_ptr, input_end, env_copy);
		if (! right)
		{
			free_cmd(cmd);
			return (NULL);
		}
		if (is_empty_command(cmd) || is_empty_command(right))
		{
			print_syntax_error();
			free_cmd(cmd);
			free_cmd(right);
			return (NULL);
		}
		cmd = pipecmd(cmd, right);
		if (! cmd)
			return (NULL);
	}
	return (cmd);
}

struct s_cmd	*parse_line(char **input_ptr, char *input_end, char **env_copy)
{
	struct s_cmd	*cmd;

	cmd = parse_pipe(input_ptr, input_end, env_copy);
	if (! cmd)
		return (NULL);
	if (peek(input_ptr, input_end, "&"))
	{
		gettoken(input_ptr, input_end, NULL, NULL);
		cmd = backcmd(cmd);
		if (! cmd)
			return (NULL);
		if (! peek(input_ptr, input_end, "\0"))
		{
			return (parse_line(input_ptr, input_end, env_copy));
		}
	}
	return (cmd);
}

struct s_cmd	*parse_block(char **input_ptr, char *input_end, char **env_copy)
{
	struct s_cmd	*cmd;

	if (! peek(input_ptr, input_end, "("))
	{
		print_missing_paren("(");
		return (NULL);
	}
	gettoken(input_ptr, input_end, NULL, NULL);
	cmd = parse_line(input_ptr, input_end, env_copy);
	if (! cmd)
		return (NULL);
	if (! peek(input_ptr, input_end, ")"))
	{
		print_missing_paren(")");
		free_cmd(cmd);
		return (NULL);
	}
	gettoken(input_ptr, input_end, NULL, NULL);
	return (cmd);
}

struct s_cmd	*parseexec(char **input_ptr, char *input_end, char **env_copy)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	int					argc;

	argc = 0;
	if (peek(input_ptr, input_end, "("))
		return (parse_block(input_ptr, input_end, env_copy));
	ret = init_exec_cmd();
	if (! ret)
		return (NULL);
	cmd = (struct s_execcmd *)ret;
	ret = process_arguments_and_redirs(ret,
			(t_process_args_params){cmd, input_ptr, input_end, &argc},
			env_copy);
	if (! ret)
		return (NULL);
	finalize_exec_cmd(cmd, argc);
	return (ret);
}
