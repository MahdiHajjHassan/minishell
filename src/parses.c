#include "minishell.h"

struct cmd *parse_pipe(char **input_ptr, char *input_end)
{
    struct cmd *cmd;
    
    cmd = parseexec(input_ptr, input_end);
    if (peek(input_ptr, input_end, "|"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = pipecmd(cmd, parse_pipe(input_ptr, input_end));
    }
    return cmd;
}

struct cmd *parse_line(char **input_ptr, char *input_end)
{
    struct cmd *cmd;

    cmd = parse_pipe(input_ptr, input_end);
    while(peek(input_ptr, input_end, "&"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = backcmd(cmd);
    }
    if (peek(input_ptr, input_end, ";"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = listcmd(cmd, listcmd(cmd, parse_line(input_ptr, input_end)));
    }
    return cmd;
}

struct cmd *parse_block(char **input_ptr, char *input_end)
{
    struct cmd *cmd;

    if (!peek(input_ptr, input_end, "("))
        wtf("missing (");
    gettoken(input_ptr, input_end, NULL, NULL);
    cmd = parse_line(input_ptr, input_end);
    if (!peek(input_ptr, input_end, ")"))
        wtf("missing )");
    gettoken(input_ptr, input_end, NULL, NULL);
    cmd = parse_redirs(cmd, input_ptr, input_end);
    return cmd;
}

struct cmd *parse_redirs(struct cmd *cmd, char **input_ptr, char *input_end)
{
    int tok;
    char    *q;
    char    *eq;

    while(peek(input_ptr, input_end, "<>"))
    {
        tok = gettoken(input_ptr, input_end, NULL, NULL);
        if (gettoken(input_ptr, input_end, &q, &eq) != 'a')
            wtf("missing file name");
        switch(tok)
        {
            case '<':
                cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
                break;
            case '>':
                cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
                break;
            case '+':
                cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1);
                break;
        }
    }
    return cmd;
}

