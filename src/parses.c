#include "minishell.h"
#include <stdio.h>
#include <string.h>

/*
 * process_escaped - Handle escaped characters in input
 * 
 * Processes escape sequences like \n, \t, etc. and converts them
 * to their actual characters. Also handles backslash escaping.
 * 
 * @input: Input string containing escape sequences
 * @len: Length of input string
 * 
 * Returns: Newly allocated string with processed escapes
 */
static char* process_escaped(const char* input, size_t len)
{
    char* output = malloc(len + 1);
    size_t i = 0;
    size_t j = 0;
    
    if (!output)
        return NULL;
        
    while (i < len) {
        if (input[i] == '\\' && i + 1 < len) {
            // Handle escaped characters
            switch (input[i + 1]) {
                case 'n': output[j++] = '\n'; break;
                case 't': output[j++] = '\t'; break;
                case 'r': output[j++] = '\r'; break;
                case 'v': output[j++] = '\v'; break;
                case 'b': output[j++] = '\b'; break;
                case 'f': output[j++] = '\f'; break;
                case 'a': output[j++] = '\a'; break;
                case '\\': output[j++] = '\\'; break;
                default: output[j++] = input[i + 1]; break;
            }
            i += 2;
        } else {
            output[j++] = input[i++];
        }
    }
    output[j] = '\0';
    return output;
}

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
struct s_cmd *parse_redirs(struct s_cmd *cmd, char **input_ptr, char *input_end)
{
    int tok;
    char *q;
    char *eq;
    char *file;
    char *processed;
    char *expanded;
    size_t len;

    while(peek(input_ptr, input_end, "<>"))
    {
        tok = gettoken(input_ptr, input_end, NULL, NULL);
        if (gettoken(input_ptr, input_end, &q, &eq) != 'a') {
            fprintf(stderr, "missing file name\n");
            wtf();
        }
        // Handle quoted filenames
        if (*q == '"' && *(eq-1) == '"') {
            q++;
            eq--;
        }
        len = eq - q;
        processed = process_escaped(q, len);
        if (!processed) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        
        // Expand environment variables in filename
        expanded = expand_variables(processed, strlen(processed));
        free(processed);
        if (!expanded) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        file = expanded;
        
        switch(tok)
        {
            case '<':  // Input redirection
                cmd = redircmd(cmd, file, file + strlen(file), O_RDONLY, 0);
                break;
            case '>':  // Output redirection (truncate)
                cmd = redircmd(cmd, file, file + strlen(file), 
                             O_WRONLY | O_CREAT | O_TRUNC | 0644, 1);
                break;
            case '+':  // Output redirection (append)
                cmd = redircmd(cmd, file, file + strlen(file),
                             O_WRONLY | O_CREAT | O_APPEND | 0644, 1);
                break;
        }
    }
    return cmd;
}

/*
 * parseexec - Parse a simple command with arguments
 * 
 * Handles simple commands like: ls -l /home
 * Also processes argument escapes and environment variables
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * 
 * Returns: Command tree for execution
 */
struct s_cmd *parseexec(char **input_ptr, char *input_end)
{
    struct s_execcmd *cmd;
    char *q;
    char *eq;
    struct s_cmd *ret;
    int argc = 0;
    int tok;
    size_t len;
    char *processed;
    char *expanded;

    if (peek(input_ptr, input_end, "("))
        return parse_block(input_ptr, input_end);
    ret = execcmd();
    cmd = (struct s_execcmd *)ret;
    ret = parse_redirs(ret, input_ptr, input_end);
    while (!peek(input_ptr, input_end, "|)&;"))
    {
        if((tok = gettoken(input_ptr, input_end, &q, &eq)) == 0)
            break;
        if(tok != 'a') {
            fprintf(stderr, "missing file name\n");
            wtf();
        }
        // Process argument string
        len = eq - q;
        processed = process_escaped(q, len);
        if (!processed) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        
        // Expand environment variables
        expanded = expand_variables(processed, strlen(processed));
        free(processed);
        if (!expanded) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        cmd->av[argc] = expanded;
        cmd->eav[argc] = cmd->av[argc] + strlen(expanded);  // Point to the null terminator
        fprintf(stderr, "DEBUG: parsed arg[%d] = '%s'\n", argc, cmd->av[argc]);
        argc++;
        if(argc >= MAXARGS) {
            fprintf(stderr, "too many args\n");
            wtf();
        }
        ret = parse_redirs(ret, input_ptr, input_end);
    }
    cmd->av[argc] = 0;
    cmd->eav[argc] = 0;
    return ret;
}
