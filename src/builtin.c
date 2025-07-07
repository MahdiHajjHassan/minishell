#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int is_builtin(char *cmd)
{
    return (strcmp(cmd, "cd") == 0);
}

int handle_builtin(char **argv)
{
    if (strcmp(argv[0], "cd") == 0)
    {
        if (!argv[1])
        {
            fprintf(stderr, "cd: missing argument\n");
            return 1;
        }
        if (chdir(argv[1]) < 0)
        {
            fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
            return 1;
        }
        return 0;
    }
    return -1;  // Not a builtin
} 