#include "minishell.h"

/*
 * handle_back_cmd - Handle BACK command type
 *
 * @cmd: Command structure to execute
 */
void	handle_back_cmd(struct s_cmd *cmd)
{
	struct s_backcmd	*back;

	back = (struct s_backcmd *)cmd;
	if (forkk() == 0)
	{
		if (forkk() == 0)
		{
			runcmd(back->cmd);
		}
		exit(0);
	}
	wait(0);
}

/*
 * handle_heredoc_cmd - Handle HEREDOC command type
 *
 * @cmd: Command structure to execute
 */
void	handle_heredoc_cmd(struct s_cmd *cmd)
{
	struct s_heredoccmd	*hcmd;
	char				temp_filename[64];
	int					temp_fd;
	char				line[1024];
	ssize_t				read_len;
	int					line_pos;
	int					c;

	hcmd = (struct s_heredoccmd *)cmd;
	ft_strlcpy(temp_filename, "/tmp/minishell_heredoc", sizeof(temp_filename));
	temp_fd = open(temp_filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (temp_fd < 0)
	{

		ft_strlcat(temp_filename, "_tmp", sizeof(temp_filename));
		temp_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	}
	if (temp_fd < 0)
	{
		perror("temp file creation failed");
		exit(1);
	}
	printf("> ");
	while (1)
	{
		line_pos = 0;

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

		if (ft_strlen(line) == ft_strlen(hcmd->delimiter) && ft_strncmp(line,
				hcmd->delimiter, ft_strlen(hcmd->delimiter)) == 0)
		{
			break ;
		}

		write(temp_fd, line, line_pos);
		write(temp_fd, "\n", 1);
		printf("> ");
	}
	close(temp_fd);
	temp_fd = open(temp_filename, O_RDONLY);
	if (temp_fd < 0)
	{
		perror("open temp file failed");
		exit(1);
	}
	dup2(temp_fd, 0);
	close(temp_fd);
	unlink(temp_filename);
	runcmd(hcmd->cmd);
}
