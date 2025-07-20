#include "minishell.h"

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
int builtin_cd(char **argv)
{
    if (!argv[1]) {
        char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return 1;
        }
        if (chdir(home) != 0) {
            fprintf(stderr, "minishell: cd: %s: %s\n", home, strerror(errno));
            return 1;
        }
    } else if (argv[2]) {
        fprintf(stderr, "minishell: cd: too many arguments\n");
        return 1;
    } else {
        if (chdir(argv[1]) != 0) {
            fprintf(stderr, "minishell: cd: %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }
    return 0;
}

/*
 * remove_quotes - Remove surrounding quotes from string
 * 
 * @value: String to process
 * 
 * Returns: Processed string (modifies original)
 */
static char *remove_quotes(char *value)
{
    size_t len = ft_strlen(value);
    
    if (len >= 2) {
        if ((*value == '"' && value[len-1] == '"') ||
            (*value == '\'' && value[len-1] == '\'')) {
            value++;
            value[len-2] = '\0';
        }
    }
    return value;
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
int builtin_export(char **argv)
{
    int i = 1;
    char *equals;
    char *name;
    char *value;

    while (argv[i]) {
        equals = ft_strchr(argv[i], '=');
        if (!equals) {
            fprintf(stderr, "minishell: export: invalid format: %s\n", argv[i]);
            return 1;
        }
        name = argv[i];
        *equals = '\0';  // Split string at '='
        value = equals + 1;
        
        // Remove surrounding quotes if present
        value = remove_quotes(value);
        
        if (setenv(name, value, 1) != 0) {
            fprintf(stderr, "minishell: export: %s\n", strerror(errno));
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
int builtin_unset(char **argv)
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