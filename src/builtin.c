#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * is_builtin - Check if a command is a shell built-in
 * 
 * @cmd: Command name to check
 * 
 * Returns:
 *   1 if command is built-in, 0 otherwise
 */
int is_builtin(char *cmd)
{
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
            !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
            !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
            !strcmp(cmd, "exit"));
}

/*
 * builtin_echo - Implement echo command
 * Supports -n option to suppress trailing newline
 * 
 * @argv: Argument array (argv[0] is "echo")
 * 
 * Example: echo -n hello world
 * Returns: 0 on success
 */
static int builtin_echo(char **argv)
{
    int i = 1;
    int print_newline = 1;

    if (argv[1] && !strcmp(argv[1], "-n")) {
        print_newline = 0;
        i++;
    }

    while (argv[i]) {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (print_newline)
        printf("\n");
    return 0;
}

/*
 * builtin_cd - Change current directory
 * Supports absolute and relative paths
 * 
 * @argv: Argument array (argv[0] is "cd")
 * 
 * Examples:
 *   cd          - Change to HOME directory
 *   cd path     - Change to specified path
 * 
 * Returns: 0 on success, 1 on error
 */
static int builtin_cd(char **argv)
{
    if (!argv[1]) {
        char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home) != 0) {
            perror("cd");
            return 1;
        }
    } else if (argv[2]) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    } else {
        if (chdir(argv[1]) != 0) {
            perror("cd");
            return 1;
        }
    }
    return 0;
}

/*
 * builtin_pwd - Print current working directory
 * 
 * @argv: Argument array (argv[0] is "pwd")
 * 
 * Returns: 0 on success, 1 on error
 */
static int builtin_pwd(char **argv)
{
    char cwd[1024];

    (void)argv; // Unused parameter
    if (!getcwd(cwd, sizeof(cwd))) {
        perror("pwd");
        return 1;
    }
    printf("%s\n", cwd);
    return 0;
}

/*
 * builtin_export - Set environment variables
 * Format: export NAME=value
 * 
 * @argv: Argument array (argv[0] is "export")
 * 
 * Example: export PATH=/usr/bin:/bin
 * Returns: 0 on success, 1 on error
 */
static int builtin_export(char **argv)
{
    int i = 1;
    char *equals;

    while (argv[i]) {
        equals = strchr(argv[i], '=');
        if (!equals) {
            fprintf(stderr, "export: invalid format: %s\n", argv[i]);
            return 1;
        }
        *equals = '\0';  // Split string at '='
        if (setenv(argv[i], equals + 1, 1) != 0) {
            perror("export");
            return 1;
        }
        i++;
    }
    return 0;
}

/*
 * builtin_unset - Remove environment variables
 * 
 * @argv: Argument array (argv[0] is "unset")
 * 
 * Example: unset PATH
 * Returns: 0 on success, 1 on error
 */
static int builtin_unset(char **argv)
{
    int i = 1;

    while (argv[i]) {
        if (unsetenv(argv[i]) != 0) {
            perror("unset");
            return 1;
        }
        i++;
    }
    return 0;
}

/*
 * builtin_env - Print all environment variables
 * 
 * @argv: Argument array (argv[0] is "env")
 * 
 * Returns: 0 on success
 */
static int builtin_env(char **argv)
{
    extern char **environ;
    int i = 0;

    (void)argv; // Unused parameter
    while (environ[i]) {
        printf("%s\n", environ[i]);
        i++;
    }
    return 0;
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
static int builtin_exit(char **argv)
{
    int status = 0;

    if (argv[1]) {
        status = atoi(argv[1]);
    }
    printf("exit\n");
    exit(status);
}

/*
 * handle_builtin - Execute built-in commands
 * 
 * @argv: Argument array (argv[0] is the command name)
 * 
 * Returns:
 *   - Return value from the built-in command
 *   - 1 if command is not a built-in
 */
int handle_builtin(char **argv)
{
    if (!argv[0])
        return 1;

    if (!strcmp(argv[0], "echo"))
        return builtin_echo(argv);
    if (!strcmp(argv[0], "cd"))
        return builtin_cd(argv);
    if (!strcmp(argv[0], "pwd"))
        return builtin_pwd(argv);
    if (!strcmp(argv[0], "export"))
        return builtin_export(argv);
    if (!strcmp(argv[0], "unset"))
        return builtin_unset(argv);
    if (!strcmp(argv[0], "env"))
        return builtin_env(argv);
    if (!strcmp(argv[0], "exit"))
        return builtin_exit(argv);

    return 1;  // Not a builtin command
} 