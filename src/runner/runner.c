#include "minishell.h"

/*
 * runcmd - Execute a command structure
 *
 * This is the core execution function that handles all command types:
 * - EXEC: Simple command execution (ls -l)
 * - REDIR: Input/output redirection (cmd > file)
 * - LIST: Sequential commands (cmd1 ; cmd2)
 * - PIPE: Pipeline between commands (cmd1 | cmd2)
 * - BACK: Background execution (cmd &)
 *
 * @cmd: Command structure to execute
 */
void	runcmd(struct s_cmd *cmd)
{
	if (cmd == 0)
		exit(0);
	if (cmd->type == EXEC)
		handle_exec_cmd(cmd);
	else if (cmd->type == REDIR)
		handle_redir_cmd(cmd);
	else if (cmd->type == LIST)
	{
		handle_list_cmd(cmd);
		return ;
	}
	else if (cmd->type == PIPE)
		handle_pipe_cmd(cmd);
	else if (cmd->type == BACK)
		handle_back_cmd(cmd);
	else if (cmd->type == HEREDOC)
		handle_heredoc_cmd(cmd);
	else
	{
		fprintf(stderr, "unknown command type: %d\n", cmd->type);
		exit(1);
	}
	if (cmd->type != LIST)
		exit(0);
}
