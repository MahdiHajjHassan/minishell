#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

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
                             O_WRONLY | O_CREAT | O_TRUNC, 1);
                break;
            case '+':  // Output redirection (append)
                cmd = redircmd(cmd, file, file + strlen(file),
                             O_WRONLY | O_CREAT | O_APPEND, 1);
                break;
            case 'h':  // Here document
                {
                    // Check if delimiter is quoted
                    char *delimiter = file;
                    int expand_vars = 1;  // Default: expand variables
                    
                    // If delimiter is quoted, remove quotes and disable expansion
                    if ((*delimiter == '"' && delimiter[strlen(delimiter)-1] == '"') ||
                        (*delimiter == '\'' && delimiter[strlen(delimiter)-1] == '\'')) {
                        expand_vars = 0;
                        delimiter = strdup(delimiter + 1);
                        delimiter[strlen(delimiter)-1] = '\0';
                    } else {
                        delimiter = strdup(delimiter);
                    }
                    
                    // Create a temporary file to store here document content
                    char temp_filename[] = "/tmp/minishell_heredoc_XXXXXX";
                    int temp_fd = mkstemp(temp_filename);
                    
                    if (temp_fd < 0) {
                        fprintf(stderr, "mkstemp failed\n");
                        wtf();
                    }
                    
                    // Read lines until we find the delimiter
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t read;
                    
                    printf("> ");
                    fflush(stdout);
                    
                    while ((read = getline(&line, &len, stdin)) != -1) {
                        // Remove newline for comparison
                        if (line[read - 1] == '\n') {
                            line[read - 1] = '\0';
                            read--;
                        }
                        
                        // Check if this is the delimiter
                        if (strcmp(line, delimiter) == 0) {
                            break;
                        }
                        
                        // Process the line based on expansion setting
                        char *processed_line;
                        if (expand_vars) {
                            processed_line = expand_variables(line, strlen(line));
                        } else {
                            processed_line = strdup(line);
                        }
                        
                        // Write line to temp file (restore newline)
                        write(temp_fd, processed_line, strlen(processed_line));
                        write(temp_fd, "\n", 1);
                        
                        free(processed_line);
                        printf("> ");
                        fflush(stdout);
                    }
                    
                    free(line);
                    free(delimiter);
                    close(temp_fd);
                    
                    // Use the temp file as input redirection
                    cmd = redircmd(cmd, strdup(temp_filename), strdup(temp_filename) + strlen(temp_filename), O_RDONLY, 0);
                }
                break;
        }
    }
    return cmd;
}

/*
 * process_quotes_robust - Robust quote processing with concatenation support
 * 
 * Handles complex quote scenarios including:
 * - Quote concatenation: 'a'"b"'c' -> abc
 * - Mixed quotes with variables: '"'"$USER"'"' -> "mahdi"
 * - Proper variable expansion in different quote contexts
 * - Edge cases like empty quotes, unmatched quotes
 * 
 * @input: Input string with quotes
 * @len: Length of input string
 * 
 * Returns: Processed string with quotes handled and variables expanded
 */
static char *process_quotes_robust(char *input, int len)
{
    char *result = malloc(len * 8 + 1);  // Extra space for expansions
    if (!result) return NULL;
    
    char *out = result;
    char *in = input;
    char *end = input + len;
    char quote_char = 0;
    int in_quotes = 0;
    
    while (in < end) {
        if (!in_quotes) {
            // Not currently in quotes
            if (*in == '"') {
                quote_char = '"';
                in_quotes = 1;
                in++;
            } else if (*in == '\'') {
                quote_char = '\'';
                in_quotes = 1;
                in++;
            } else {
                // Unquoted character - handle escapes and variable expansion
                if (*in == '\\' && (in + 1) < end) {
                    // Handle escape sequences in unquoted text
                    char next_char = *(in + 1);
                    switch (next_char) {
                        case 'n': *out++ = '\n'; break;
                        case 't': *out++ = '\t'; break;
                        case 'r': *out++ = '\r'; break;
                        case 'v': *out++ = '\v'; break;
                        case 'b': *out++ = '\b'; break;
                        case 'f': *out++ = '\f'; break;
                        case 'a': *out++ = '\a'; break;
                        case '\\': *out++ = '\\'; break;
                        default: *out++ = next_char; break;
                    }
                    in += 2;
                } else if (*in == '$') {
                    // Variable expansion outside quotes
                    char *var_start = in;
                    char *var_end = in + 1;
                    
                    if (var_end < end && *var_end == '{') {
                        // ${var} format
                        var_end++;
                        while (var_end < end && *var_end != '}') {
                            var_end++;
                        }
                        if (var_end < end && *var_end == '}') {
                            var_end++;
                        }
                    } else if (var_end < end && (isalnum(*var_end) || *var_end == '_')) {
                        // $var format
                        while (var_end < end && (isalnum(*var_end) || *var_end == '_')) {
                            var_end++;
                        }
                    }
                    
                    if (var_end > var_start + 1) {
                        // We have a variable to expand
                        char var_buffer[256];
                        int var_len = var_end - var_start;
                        if (var_len < 255) {
                            strncpy(var_buffer, var_start, var_len);
                            var_buffer[var_len] = '\0';
                            
                            char *expanded = expand_variables(var_buffer, var_len);
                            if (expanded) {
                                strcpy(out, expanded);
                                out += strlen(expanded);
                                free(expanded);
                            }
                            in = var_end;
                        } else {
                            *out++ = *in++;
                        }
                    } else {
                        *out++ = *in++;
                    }
                } else {
                    *out++ = *in++;
                }
            }
        } else {
            // Currently in quotes
            if (*in == quote_char) {
                // End of current quote
                in_quotes = 0;
                quote_char = 0;
                in++;
            } else {
                // Character inside quotes
                if (quote_char == '"' && *in == '\\' && (in + 1) < end) {
                    // Handle escape sequences inside double quotes
                    char next_char = *(in + 1);
                    if (next_char == '"' || next_char == '\\' || next_char == '$') {
                        // Escaped special characters - treat as literal
                        *out++ = next_char;
                        in += 2;  // Skip both \ and the escaped character
                    } else {
                        // Not a special escape sequence, keep the backslash
                        *out++ = *in++;
                    }
                } else if (quote_char == '"' && *in == '$') {
                    // Variable expansion inside double quotes
                    char *var_start = in;
                    char *var_end = in + 1;
                    
                    if (var_end < end && *var_end == '{') {
                        // ${var} format
                        var_end++;
                        while (var_end < end && *var_end != '}') {
                            var_end++;
                        }
                        if (var_end < end && *var_end == '}') {
                            var_end++;
                        }
                    } else if (var_end < end && (isalnum(*var_end) || *var_end == '_')) {
                        // $var format
                        while (var_end < end && (isalnum(*var_end) || *var_end == '_')) {
                            var_end++;
                        }
                    }
                    
                    if (var_end > var_start + 1) {
                        // We have a variable to expand
                        char var_buffer[256];
                        int var_len = var_end - var_start;
                        if (var_len < 255) {
                            strncpy(var_buffer, var_start, var_len);
                            var_buffer[var_len] = '\0';
                            
                            char *expanded = expand_variables(var_buffer, var_len);
                            if (expanded) {
                                strcpy(out, expanded);
                                out += strlen(expanded);
                                free(expanded);
                            }
                            in = var_end;
                        } else {
                            *out++ = *in++;
                        }
                    } else {
                        *out++ = *in++;
                    }
                } else {
                    // Regular character inside quotes (no expansion in single quotes)
                    *out++ = *in++;
                }
            }
        }
    }
    
    // Check for unclosed quotes
    if (in_quotes) {
        free(result);
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        return NULL;
    }
    
    *out = '\0';
    return result;
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
        
        len = eq - q;
        // Skip process_escaped and handle all escaping in quote processing
        // processed = process_escaped(q, len);
        // if (!processed) {
        //     fprintf(stderr, "malloc failed\n");
        //     wtf();
        // }
        
        // Use robust quote processing that handles all edge cases
        expanded = process_quotes_robust(q, len);
        if (!expanded) {
            // Error already printed by process_quotes_robust
            wtf();
        }
        
        // Check if this is a variable assignment (contains '=')
        if (strchr(expanded, '=') != NULL && argc == 0) {
            // This might be a variable assignment
            char *equals = strchr(expanded, '=');
            char *name = expanded;
            
            // Check if everything before '=' is a valid variable name
            int is_valid_var = 1;
            for (char *p = name; p < equals; p++) {
                if (!isalnum(*p) && *p != '_') {
                    is_valid_var = 0;
                    break;
                }
            }
            
            if (is_valid_var && name < equals) {
                // This is a variable assignment
                *equals = '\0';  // Split at '='
                char *value = equals + 1;
                
                if (setenv(name, value, 1) != 0) {
                    fprintf(stderr, "setenv failed\n");
                    wtf();
                }
                free(expanded);
                
                // Continue parsing to see if there's a command after the assignment
                continue;
            }
        }
        
        // Regular argument
        cmd->av[argc] = expanded;
        argc++;
        if(argc >= MAXARGS) {
            fprintf(stderr, "too many args\n");
            wtf();
        }
        ret = parse_redirs(ret, input_ptr, input_end);
    }
    cmd->av[argc] = 0;
    return ret;
}
