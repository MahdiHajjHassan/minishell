#include "minishell.h"

/*
 * heredoccmd - Create a new here document command structure
 *
 * Sets up a here document redirection.
 * Used for commands like: cat << EOF
 *
 * @subcmd: Command to receive the input
 * @delimiter: Delimiter string to mark end of input
 * @fd: File descriptor to redirect (usually 0 for stdin)
 *
 * Returns: New here document command structure
 */
struct s_cmd	*heredoccmd(struct s_cmd *subcmd, char *delimiter, int fd)
{
	struct s_heredoccmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = HEREDOC;
	cmd->cmd = subcmd;
	cmd->delimiter = delimiter;
	cmd->fd = fd;
	return ((struct s_cmd *)cmd);
}
