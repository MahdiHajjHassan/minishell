#include "minishell.h"

/*
 * builtin_echo - Implement echo command
 * Supports -n option to suppress trailing newline
 *
 * @argv: Argument array (argv[0] is "echo")
 *
 * Example: echo -n hello world
 * Returns: 0 on success
 */
int	builtin_echo(char **argv)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	if (argv[1] && ft_strlen(argv[1]) == 2 && !ft_strncmp(argv[1], "-n", 2))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
	return (0);
}

/*
 * builtin_pwd - Print current working directory
 *
 * @argv: Argument array (argv[0] is "pwd")
 *
 * Returns: 0 on success, 1 on error
 */
int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

/*
 * builtin_env - Print all environment variables
 *
 * @argv: Argument array (argv[0] is "env")
 *
 * Returns: 0 on success
 */
int	builtin_env(char **argv)
{
	extern char	**environ;
	int			i;

	i = 0;
	(void)argv;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

/*
 * check_numeric_arg - Check if argument is numeric
 *
 * @arg: Argument to check
 *
 * Returns: 1 if numeric, 0 otherwise
 */
static int	check_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

/*
 * builtin_exit - Exit the shell with optional status
 *
 * @argv: Argument array (argv[0] is "exit")
 *
 * Examples:
 *   exit    - Exit with status 0
 *   exit 1  - Exit with status 1
 */
int	builtin_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1])
	{
		if (!check_numeric_arg(argv[1]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				argv[1]);
			exit(255);
		}
		status = ft_atoi(argv[1]);
	}
	printf("exit\n");
	exit(status);
}
