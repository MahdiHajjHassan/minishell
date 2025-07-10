#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Check if command is a built-in
int is_builtin(char *cmd)
{
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
            !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
            !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
            !strcmp(cmd, "exit"));
}

// Built-in: echo with -n option
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

// Built-in: cd with relative or absolute path
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

// Built-in: pwd with no options
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

// Built-in: export with no options
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

// Built-in: unset with no options
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

// Built-in: env with no options or arguments
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

// Built-in: exit with no options
static int builtin_exit(char **argv)
{
    int status = 0;

    if (argv[1]) {
        status = atoi(argv[1]);
    }
    printf("exit\n");
    exit(status);
}

// Handle built-in commands
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