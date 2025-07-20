#include "minishell.h"

/*
 * str_equals - Compare two strings for exact equality using libft
 * 
 * @s1: First string
 * @s2: Second string
 * 
 * Returns: 1 if equal, 0 if different
 */
static int str_equals(const char *s1, const char *s2)
{
    size_t len1, len2;
    
    if (!s1 || !s2)
        return (s1 == s2);
        
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    
    if (len1 != len2)
        return 0;
        
    return (ft_strncmp(s1, s2, len1) == 0);
}

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
    return (str_equals(cmd, "echo") || str_equals(cmd, "cd") ||
            str_equals(cmd, "pwd") || str_equals(cmd, "export") ||
            str_equals(cmd, "unset") || str_equals(cmd, "env") ||
            str_equals(cmd, "exit"));
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

    if (str_equals(argv[0], "echo"))
        return builtin_echo(argv);
    if (str_equals(argv[0], "cd"))
        return builtin_cd(argv);
    if (str_equals(argv[0], "pwd"))
        return builtin_pwd(argv);
    if (str_equals(argv[0], "export"))
        return builtin_export(argv);
    if (str_equals(argv[0], "unset"))
        return builtin_unset(argv);
    if (str_equals(argv[0], "env"))
        return builtin_env(argv);
    if (str_equals(argv[0], "exit"))
        return builtin_exit(argv);

    return 1;  // Not a builtin command
} 