#include "minishell.h"

/*
 * handle_input_redir - Handle input redirection (<)
 *
 * @cmd: Command to redirect
 * @file: File to redirect from
 *
 * Returns: Modified command with redirection
 */
static struct s_cmd	*handle_input_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + ft_strlen(file), O_RDONLY, 0));
}

/*
 * handle_output_redir - Handle output redirection (>)
 *
 * @cmd: Command to redirect
 * @file: File to redirect to
 *
 * Returns: Modified command with redirection
 */
static struct s_cmd	*handle_output_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + ft_strlen(file),
			O_WRONLY | O_CREAT | O_TRUNC, 1));
}

/*
 * handle_append_redir - Handle append redirection (>>)
 *
 * @cmd: Command to redirect
 * @file: File to append to
 *
 * Returns: Modified command with redirection
 */
static struct s_cmd	*handle_append_redir(struct s_cmd *cmd, char *file)
{
	return (redircmd(cmd, file, file + ft_strlen(file),
			O_WRONLY | O_CREAT | O_APPEND, 1));
}

/*
 * create_heredoc_temp - Create temporary file for heredoc
 *
 * @delimiter: Delimiter string
 * @expand_vars: Whether to expand variables
 *
 * Returns: Temporary filename, or NULL on error
 */
static char	*create_heredoc_temp(char *delimiter, int expand_vars)
{
	char	temp_filename[64];
	int		temp_fd;
	char	line[1024];
	ssize_t	read_len;
	int		line_pos;
	int		c;
		char *processed_line;

	// Create unique temp filename using simple approach
	ft_strlcpy(temp_filename, "/tmp/minishell_heredoc_parse",
		sizeof(temp_filename));
	temp_fd = open(temp_filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (temp_fd < 0)
	{
		// If file exists, try with different suffix
		ft_strlcpy(temp_filename, "/tmp/minishell_heredoc_parse_tmp",
			sizeof(temp_filename));
		temp_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	}
	if (temp_fd < 0)
	{
		fprintf(stderr, "temp file creation failed\n");
		return (NULL);
	}
	// Read lines until we find the delimiter
	printf("> ");
	while (1)
	{
		line_pos = 0;
		// Read line character by character using read()
		while (line_pos < 1023)
		{
			read_len = read(0, &c, 1);
			if (read_len <= 0)
				break ;
			if (c == '\n')
				break ;
			line[line_pos++] = c;
		}
		if (read_len <= 0)
			break ;
		line[line_pos] = '\0';
		// Check if this is the delimiter
		if (ft_strlen(line) == ft_strlen(delimiter) && ft_strncmp(line,
				delimiter, ft_strlen(delimiter)) == 0)
		{
			break ;
		}
		// Process the line based on expansion setting
		if (expand_vars)
		{
			processed_line = expand_variables(line, ft_strlen(line));
		}
		else
		{
			processed_line = ft_strdup(line);
		}
		// Write line to temp file
		write(temp_fd, processed_line, ft_strlen(processed_line));
		write(temp_fd, "\n", 1);
		free(processed_line);
		printf("> ");
	}
	close(temp_fd);
	return (ft_strdup(temp_filename));
}

/*
 * handle_heredoc_redir - Handle here document redirection (<<)
 *
 * @cmd: Command to redirect
 * @file: Delimiter string
 *
 * Returns: Modified command with redirection
 */
static struct s_cmd	*handle_heredoc_redir(struct s_cmd *cmd, char *file)
{
	char	*delimiter;
	char	*temp_filename;

	delimiter = file;
	int expand_vars = 1; // Default: expand variables
	// If delimiter is quoted, remove quotes and disable expansion
	if ((*delimiter == '"' && delimiter[ft_strlen(delimiter) - 1] == '"')
		|| (*delimiter == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\''))
	{
		expand_vars = 0;
		delimiter = ft_strdup(delimiter + 1);
		delimiter[ft_strlen(delimiter) - 1] = '\0';
	}
	else
	{
		delimiter = ft_strdup(delimiter);
	}
	temp_filename = create_heredoc_temp(delimiter, expand_vars);
	free(delimiter);
	if (!temp_filename)
	{
		fprintf(stderr, "heredoc creation failed\n");
		wtf();
	}
	// Use the temp file as input redirection
	return (redircmd(cmd, temp_filename, temp_filename
			+ ft_strlen(temp_filename), O_RDONLY, 0));
}

/*
 * parse_redirs - Parse input/output redirections
 *
 * Handles redirections like:
 * - < input.txt (input redirection)
 * - > output.txt (output redirection)
 * - >> append.txt (append redirection)
 *
 * @cmd: Command to be redirected
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 *
 * Returns: Command tree with redirections
 */
struct s_cmd	*parse_redirs(struct s_cmd *cmd, char **input_ptr,
		char *input_end)
{
	int tok;
	char *q;
	char *eq;
	char *file;
	char *processed;
	char *expanded;
	size_t len;

	while (peek(input_ptr, input_end, "<>"))
	{
		tok = gettoken(input_ptr, input_end, NULL, NULL);
		if (gettoken(input_ptr, input_end, &q, &eq) != 'a')
		{
			fprintf(stderr, "missing file name\n");
			wtf();
		}
		// Handle quoted filenames
		if (*q == '"' && *(eq - 1) == '"')
		{
			q++;
			eq--;
		}
		len = eq - q;
		processed = process_escaped(q, len);
		if (!processed)
		{
			fprintf(stderr, "malloc failed\n");
			wtf();
		}

		// Expand environment variables in filename
		expanded = expand_variables(processed, ft_strlen(processed));
		free(processed);
		if (!expanded)
		{
			fprintf(stderr, "malloc failed\n");
			wtf();
		}
		file = expanded;

		if (tok == '<') // Input redirection
		{
			cmd = handle_input_redir(cmd, file);
		}
		else if (tok == '>') // Output redirection (truncate)
		{
			cmd = handle_output_redir(cmd, file);
		}
		else if (tok == '+') // Output redirection (append)
		{
			cmd = handle_append_redir(cmd, file);
		}
		else if (tok == 'h') // Here document
		{
			cmd = handle_heredoc_redir(cmd, file);
		}
	}
	return (cmd);
}