#include "minishell.h"

char   space[] = " \t\r\n\v";
char   symbols[] = "<>|&;()";

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
            s++;
            break;
        case '>':
            s++;
            if (*s == '>')
            {
                ret = '+';
                s++;
            }
            break;
        default:
            ret = 'a';
            while (s < input_end && !strchr(symbols, *s) 
                    && !strchr(space, *s))
                s++;
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
    *ps = s;
    return *s && strchr(toks, *s);
}

struct cmd *parse_line(char *s)
{
    char    *input_end;
    struct cmd *cmd;

    input_end = s + strlen(s);
    cmd = parse_line(&s, input_end);
    peek(&s, input_end, "");
    if (s != input_end)
    {
        fprintf(2, "unrecognized symbol: %s\n", s);
        wtf();
    }
    nulterm(cmd);
    return cmd;
}


