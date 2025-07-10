#include "minishell.h"
#include <stdio.h>
#include <string.h>

char   space[] = " \t\r\n\v";
char   symbols[] = "<>|&;()";

// Helper function to handle escaped characters
static int is_escaped(const char *s, const char *start)
{
    int count = 0;
    while (s > start && *(s - 1) == '\\') {
        count++;
        s--;
    }
    return count % 2;  // Odd number of backslashes means the character is escaped
}

int gettoken(char **input_ptr, char *input_end, char **token_start, char **token_end)
{
    char *s;
    int ret;

    s = *input_ptr;
    while (s < input_end && strchr(space, *s))
        s++;
    if (token_start)
        *token_start = s;
    ret = *s;
    switch (*s)
    {
        case 0:
            break;
        case '|':
        case '&':
        case ';':
        case '(':
        case ')':
        case '<':
            // Only treat as special if not escaped
            if (!is_escaped(s, *input_ptr)) {
                s++;
                break;
            }
            /* FALLTHROUGH */
        case '>':
            if (!is_escaped(s, *input_ptr)) {
                s++;
                if (*s == '>' && !is_escaped(s, *input_ptr)) {
                    ret = '+';
                    s++;
                }
                break;
            }
            /* FALLTHROUGH */
        default:
            ret = 'a';
            while (s < input_end && 
                   ((!strchr(symbols, *s) || is_escaped(s, *input_ptr)) && 
                    (!strchr(space, *s) || is_escaped(s, *input_ptr)))) {
                s++;
            }
            break;
    }
    if (token_end)
        *token_end = s;

    while (s < input_end && strchr(space, *s))
        s++;
    *input_ptr = s;
    return ret;
}

int peek(char **input_ptr, char *input_end, char *toks)
{
    char *s;

    s = *input_ptr;
    while (s < input_end && strchr(space, *s))
        s++;
    return *s && strchr(toks, *s);
}

struct s_cmd *tokenize(const char *line)
{
    fprintf(stderr, "DEBUG: tokenizing line: '%s'\n", line);
    char *input = strdup(line);
    char *input_ptr = input;
    char *input_end = input + strlen(input);
    
    // Trim trailing whitespace including newlines
    while (input_end > input && strchr(space, *(input_end - 1)))
        *(--input_end) = '\0';
    
    struct s_cmd *cmd = parse_line(&input_ptr, input_end);
    free(input);
    fprintf(stderr, "DEBUG: tokenize complete\n");
    return cmd;
}


