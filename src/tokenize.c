#include "minishell.h"
#include <stdio.h>
#include <string.h>

/* Characters considered as whitespace */
char   space[] = " \t\r\n\v";

/* Special shell syntax characters */
char   symbols[] = "<>|&;()";

/*
 * is_escaped - Check if a character is escaped with backslash
 * 
 * Counts backslashes before the character to determine if it's escaped.
 * An odd number of backslashes means the character is escaped.
 * 
 * @s: Current position in string
 * @start: Start of the string
 * 
 * Returns: 1 if character is escaped, 0 otherwise
 */
static int is_escaped(const char *s, const char *start)
{
    int count = 0;
    while (s > start && *(s - 1) == '\\') {
        count++;
        s--;
    }
    return count % 2;  // Odd number of backslashes means the character is escaped
}

/*
 * gettoken - Get next token from input string
 * 
 * Tokenizes the input string based on shell syntax rules.
 * Handles special characters, whitespace, and escaped characters.
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * @token_start: Where to store start of token
 * @token_end: Where to store end of token
 * 
 * Returns: Token type:
 *   - 0 for end of string
 *   - Special character for operators (<,>,|,&,;)
 *   - '+' for >> operator
 *   - 'a' for regular arguments
 */
int gettoken(char **input_ptr, char *input_end, char **token_start, char **token_end)
{
    char *s;
    int ret;

    s = *input_ptr;
    // Skip leading whitespace
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
                // Handle >> operator
                if (*s == '>' && !is_escaped(s, *input_ptr)) {
                    ret = '+';
                    s++;
                }
                break;
            }
            /* FALLTHROUGH */
        default:
            ret = 'a';
            // Continue until we hit a special character or whitespace
            while (s < input_end && 
                   ((!strchr(symbols, *s) || is_escaped(s, *input_ptr)) && 
                    (!strchr(space, *s) || is_escaped(s, *input_ptr)))) {
                s++;
            }
            break;
    }
    if (token_end)
        *token_end = s;

    // Skip trailing whitespace
    while (s < input_end && strchr(space, *s))
        s++;
    *input_ptr = s;
    return ret;
}

/*
 * peek - Look ahead for specific tokens
 * 
 * Checks if the next non-whitespace character matches any in toks.
 * Used to check for upcoming operators without consuming them.
 * 
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * @toks: String of characters to look for
 * 
 * Returns: 1 if next character matches any in toks, 0 otherwise
 */
int peek(char **input_ptr, char *input_end, char *toks)
{
    char *s;

    s = *input_ptr;
    while (s < input_end && strchr(space, *s))
        s++;
    return *s && strchr(toks, *s);
}

/*
 * tokenize - Convert input line to command structure
 * 
 * Main entry point for parsing shell commands.
 * Converts a string into a tree of command structures.
 * 
 * @line: Input command line
 * 
 * Returns: Root of command tree
 */
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


