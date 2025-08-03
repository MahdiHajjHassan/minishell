/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper9.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_filename_token(int filename_tok, struct s_cmd **ret,
		t_process_args_params params)
{
	(void)params;
	if (filename_tok != 'a')
	{
		print_missing_file_name();
		free_cmd(*ret);
		return (1);
	}
	return (0);
}

int	check_quoted_status(char **q, char **eq)
{
	int	was_quoted;

	if (**q == '"' && *(*eq - 1) == '"')
	{
		(*q)++;
		(*eq)--;
		was_quoted = 1;
		return (was_quoted);
	}
	else if (**q == '\'' && *(*eq - 1) == '\'')
	{
		(*q)++;
		(*eq)--;
		was_quoted = 1;
		return (was_quoted);
	}
	return (0);
}



static struct s_cmd	*replace_input_redir(struct s_cmd *cmd, char *file)
{
	struct s_redircmd	*rdir;

	if (!cmd || cmd->type != REDIR)
		return (apply_input_redir(cmd, file));
	
	rdir = (struct s_redircmd *)cmd;
	if (rdir->fd == 0)  // This is an input redirection
	{
		// Replace the file and keep the same structure
		free(rdir->file);
		rdir->file = file;
		return (cmd);
	}
	else
	{
		// This is not an input redirection, so apply the new input redirection
		rdir->cmd = replace_input_redir(rdir->cmd, file);
		return (cmd);
	}
}

static struct s_cmd	*replace_output_redir(struct s_cmd *cmd, char *file)
{
	// For output redirections, we want the last redirection to be innermost
	// So we need to find the innermost command and wrap it
	if (cmd && cmd->type == REDIR)
	{
		struct s_redircmd *rdir = (struct s_redircmd *)cmd;
		if (rdir->fd == STDOUT_FILENO)
		{
			// Find the innermost command (the one that's not a redirection)
			struct s_cmd *innermost = cmd;
			while (innermost && innermost->type == REDIR)
			{
				struct s_redircmd *inner_rdir = (struct s_redircmd *)innermost;
				if (inner_rdir->fd != STDOUT_FILENO)
					break;
				innermost = inner_rdir->cmd;
			}
			
			// Create new redirection that wraps the innermost command
			struct s_redircmd *new_rdir = (struct s_redircmd *)redircmd(innermost, file, NULL, (t_redir_params){O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO});
			
			// If this was the only redirection, return the new one
			if (cmd == innermost)
				return ((struct s_cmd *)new_rdir);
			
			// Otherwise, update the chain to point to the new innermost
			struct s_cmd *current = cmd;
			while (current && current->type == REDIR)
			{
				struct s_redircmd *current_rdir = (struct s_redircmd *)current;
				if (current_rdir->fd != STDOUT_FILENO)
					break;
				if (current_rdir->cmd == innermost)
				{
					current_rdir->cmd = (struct s_cmd *)new_rdir;
					break;
				}
				current = current_rdir->cmd;
			}
			return (cmd);
		}
	}
	return (apply_output_redir(cmd, file));
}

static struct s_cmd	*replace_append_redir(struct s_cmd *cmd, char *file)
{
	// For append redirections, we want the last redirection to be innermost
	// So we need to find the innermost command and wrap it
	if (cmd && cmd->type == REDIR)
	{
		struct s_redircmd *rdir = (struct s_redircmd *)cmd;
		if (rdir->fd == STDOUT_FILENO)
		{
			// Find the innermost command (the one that's not a redirection)
			struct s_cmd *innermost = cmd;
			while (innermost && innermost->type == REDIR)
			{
				struct s_redircmd *inner_rdir = (struct s_redircmd *)innermost;
				if (inner_rdir->fd != STDOUT_FILENO)
					break;
				innermost = inner_rdir->cmd;
			}
			
			// Create new append redirection that wraps the innermost command
			struct s_redircmd *new_rdir = (struct s_redircmd *)redircmd(innermost, file, NULL, (t_redir_params){O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO});
			
			// If this was the only redirection, return the new one
			if (cmd == innermost)
				return ((struct s_cmd *)new_rdir);
			
			// Otherwise, update the chain to point to the new innermost
			struct s_cmd *current = cmd;
			while (current && current->type == REDIR)
			{
				struct s_redircmd *current_rdir = (struct s_redircmd *)current;
				if (current_rdir->fd != STDOUT_FILENO)
					break;
				if (current_rdir->cmd == innermost)
				{
					current_rdir->cmd = (struct s_cmd *)new_rdir;
					break;
				}
				current = current_rdir->cmd;
			}
			return (cmd);
		}
	}
	return (apply_append_redir(cmd, file));
}

int	create_redirection_cmd(t_redir_cmd_params params)
{
	struct s_cmd	*result;

	if (params.tok == '<')
	{
		result = replace_input_redir(*params.ret, params.file_or_delimiter);
		*params.ret = result;
	}
	else if (params.tok == '>')
		*params.ret = replace_output_redir(*params.ret, params.file_or_delimiter);
	else if (params.tok == '+')
		*params.ret = replace_append_redir(*params.ret, params.file_or_delimiter);
	else if (params.tok == 'H')
		*params.ret = handle_heredoc_token(*params.ret,
				params.file_or_delimiter, params.env_copy, params.was_quoted);
	return (0);
}

int	process_redirection_file(t_redir_file_params params)
{
	char				*file_or_delimiter;
	int					was_quoted;
	t_redir_cmd_params	cmd_params;

	was_quoted = check_quoted_status(params.q, params.eq);
	file_or_delimiter = process_filename(*params.q, *params.eq,
			params.env_copy);
	if (!file_or_delimiter)
	{
		free_cmd(*params.ret);
		return (1);
	}
	cmd_params.ret = params.ret;
	cmd_params.file_or_delimiter = file_or_delimiter;
	cmd_params.env_copy = params.env_copy;
	cmd_params.tok = params.tok;
	cmd_params.was_quoted = was_quoted;
	create_redirection_cmd(cmd_params);
	return (0);
}
