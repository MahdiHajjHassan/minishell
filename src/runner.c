/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command(const char *cmd, char **env_copy)
{
	char	*path;
	char	*result;

	path = get_env_value("PATH", 4, env_copy);
	if (!path || ft_strlen(path) == 0)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result != (char *)1)
	{
		free(path);
		return (result);
	}
	result = search_in_paths(path, cmd);
	free(path);
	return (result);
}

void	run_exec_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_execcmd	*ex;

	ex = (struct s_execcmd *)cmd;
	if (ex->av[0] == 0)
		clean_exit(0);
	/* Variable expansion now happens during parsing with quote context awareness */
	if (is_builtin(ex->av[0]))
	{
		handle_exec_builtin(ex, cmd, &env_copy);
		return ;
	}
	execute_external_cmd(ex, env_copy);
}

void	run_redir_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_redircmd	*rdir;
	int					saved_stdin;
	int					saved_stdout;

	rdir = (struct s_redircmd *)cmd;
	
	/* Save original file descriptors */
	if (rdir->fd == STDIN_FILENO)
		saved_stdin = dup(STDIN_FILENO);
	else if (rdir->fd == STDOUT_FILENO)
		saved_stdout = dup(STDOUT_FILENO);
	
	/* Close the original file descriptor */
	close(rdir->fd);
	
	/* Open the redirection file */
	if ((rdir->mode & O_CREAT) && (rdir->mode & (O_WRONLY | O_RDWR)))
	{
		if (open_redir_file_create(rdir) != 0)
		{
			set_exit_status(1);
			/* Restore file descriptors before exiting */
			if (rdir->fd == STDIN_FILENO)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
			else if (rdir->fd == STDOUT_FILENO)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
			return;
		}
	}
	else
	{
		if (open_redir_file_regular(rdir) != 0)
		{
			set_exit_status(1);
			/* Restore file descriptors before exiting */
			if (rdir->fd == STDIN_FILENO)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
			else if (rdir->fd == STDOUT_FILENO)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
			return;
		}
	}
	
	/* Execute the command with redirection */
	runcmd(rdir->cmd, env_copy);
	
	/* Restore original file descriptors */
	if (rdir->fd == STDIN_FILENO)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	else if (rdir->fd == STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

/* List command execution removed - semicolon not supported in this minishell */
void	run_list_cmd(struct s_cmd *cmd)
{
	(void)cmd;
}

void	runcmd(struct s_cmd *cmd, char **env_copy)
{
	if (cmd == 0)
		clean_exit(0);
	if (cmd->type == EXEC)
		run_exec_cmd(cmd, env_copy);
	else if (cmd->type == REDIR)
		run_redir_cmd(cmd, env_copy);
	else if (cmd->type == HEREDOC)
		run_heredoc_cmd(cmd, env_copy);
	/* List command handling removed - semicolon not supported in this minishell */
	else if (cmd->type == PIPE)
		run_pipe_cmd(cmd, env_copy);
	else if (cmd->type == BACK)
		run_back_cmd(cmd, env_copy);
	else
	{
		ft_fprintf_stderr("unknown command type: %d\n", cmd->type);
		clean_exit(1);
	}
	// Don't call clean_exit(0) here - let the calling function handle exit
}
