/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_cmd	*execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file,
							char *efile, t_redir_params params)
{
	struct s_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = params.mode;
	cmd->fd = params.fd;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*listcmd(struct s_cmd *left, struct s_cmd *right)
{
	struct s_listcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = LIST;
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*backcmd(struct s_cmd *subcmd)
{
	struct s_backcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = BACK;
	cmd->cmd = subcmd;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*heredoccmd(struct s_cmd *subcmd, char *delimiter, char *content)
{
	struct s_heredoccmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		free(delimiter);
		free(content);
		return (NULL);
	}
	
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = HEREDOC;
	cmd->cmd = subcmd;
	cmd->delimiter = delimiter;
	cmd->content = content;
	cmd->fd = 0;
	
	return ((struct s_cmd *)cmd);
}
