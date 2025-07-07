#include "minishell.h"
#include <stdio.h>
#include <string.h>

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

struct s_cmd *parse_line(char **input_ptr, char *input_end)
{
    struct s_cmd *cmd;

    cmd = parse_pipe(input_ptr, input_end);
    if (peek(input_ptr, input_end, "&"))
    {
        gettoken(input_ptr, input_end, NULL, NULL);
        cmd = backcmd(cmd);
        // After a background command, parse the rest as a new command
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

struct s_cmd *parse_redirs(struct s_cmd *cmd, char **input_ptr, char *input_end)
{
    int tok;
    char *q;
    char *eq;
    char *file;

    while(peek(input_ptr, input_end, "<>"))
    {
        tok = gettoken(input_ptr, input_end, NULL, NULL);
        if (gettoken(input_ptr, input_end, &q, &eq) != 'a') {
            fprintf(stderr, "missing file name\n");
            wtf();
        }
        // Make a clean copy of the filename without quotes
        if (*q == '"' && *(eq-1) == '"') {
            q++;
            eq--;
        }
        file = malloc(eq - q + 1);
        if (!file) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        strncpy(file, q, eq - q);
        file[eq - q] = '\0';
        
        switch(tok)
        {
            case '<':
                cmd = redircmd(cmd, file, file + strlen(file), O_RDONLY, 0);
                break;
            case '>':
                cmd = redircmd(cmd, file, file + strlen(file), O_WRONLY | O_CREAT | O_TRUNC | 0644, 1);
                break;
            case '+':
                cmd = redircmd(cmd, file, file + strlen(file), O_WRONLY | O_CREAT | O_APPEND | 0644, 1);
                break;
        }
    }
    return cmd;
}

struct s_cmd *parseexec(char **input_ptr, char *input_end)
{
    struct s_execcmd *cmd;
    char *q;
    char *eq;
    struct s_cmd *ret;
    int argc = 0;
    int tok;
    size_t len;

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
        // Make a clean copy of the argument
        len = eq - q;
        cmd->av[argc] = malloc(len + 1);
        if (!cmd->av[argc]) {
            fprintf(stderr, "malloc failed\n");
            wtf();
        }
        strncpy(cmd->av[argc], q, len);
        cmd->av[argc][len] = '\0';
        cmd->ev[argc] = cmd->av[argc] + len;  // Point to the null terminator
        fprintf(stderr, "DEBUG: parsed arg[%d] = '%s'\n", argc, cmd->av[argc]);
        argc++;
        if(argc >= MAXARGS) {
            fprintf(stderr, "too many args\n");
            wtf();
        }
        ret = parse_redirs(ret, input_ptr, input_end);
    }
    cmd->av[argc] = 0;
    cmd->ev[argc] = 0;
    return ret;
}
