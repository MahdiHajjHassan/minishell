#include "minishell.h"

/*
 * parse_pipe - Parse a pipeline of commands
 * 
 * Handles command pipelines like: cmd1 | cmd2 | cmd3
 * Creates a tree of pipe commands connecting the outputs and inputs.
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * 
 * Returns: Command tree for the pipeline
 */
struct s_cmd *parse_pipe(char **input_ptr, char *input_end)
{
    struct s_cmd *cmd;
    
    cmd = parseexec(input_ptr, input_end);
    if (peek(input_ptr, input_end, "|"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = pipecmd(cmd, parse_pipe(input_ptr, input_end));
    }
    return cmd;
}

/*
 * parse_line - Parse a complete command line
 * 
 * Handles command sequences separated by ; and background &
 * Example: cmd1 ; cmd2 & cmd3
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * 
 * Returns: Command tree for the entire line
 */
struct s_cmd *parse_line(char **input_ptr, char *input_end)
{
    struct s_cmd *cmd;

    cmd = parse_pipe(input_ptr, input_end);
    if (peek(input_ptr, input_end, "&"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = backcmd(cmd);
        if (!peek(input_ptr, input_end, "\0")) {
            return parse_line(input_ptr, input_end);
        }
    }
    else if (peek(input_ptr, input_end, ";"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = listcmd(cmd, parse_line(input_ptr, input_end));
    }
    return cmd;
}

/*
 * parse_block - Parse a parenthesized command block
 * 
 * Handles commands in parentheses: (cmd)
 * Allows for command grouping and redirection of groups
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * 
 * Returns: Command tree for the block
 */
struct s_cmd *parse_block(char **input_ptr, char *input_end)
{
    struct s_cmd *cmd;

    if (!peek(input_ptr, input_end, "(")) {
        fprintf(stderr, "missing (\n");
        wtf();
    }
    gettoken(input_ptr, input_end, NULL, NULL);
    cmd = parse_line(input_ptr, input_end);
    if (!peek(input_ptr, input_end, ")")) {
        fprintf(stderr, "missing )\n");
        wtf();
    }
    gettoken(input_ptr, input_end, NULL, NULL);
    cmd = parse_redirs(cmd, input_ptr, input_end);
    return cmd;
} 