#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/* Signal handling structure */
typedef struct s_sig
{
    volatile sig_atomic_t sigint;    // SIGINT received
    volatile sig_atomic_t sigquit;   // SIGQUIT received
    volatile pid_t pid;              // Current foreground process PID
    volatile int exit_status;        // Exit status from signal
} t_sig;

extern t_sig g_sig;  // Global signal state

/* Command Types - Used to identify different command structures */
#define EXEC 1   // Simple command execution (e.g., ls -l)
#define REDIR 2  // Redirection (e.g., ls > file.txt)
#define PIPE 3   // Pipeline between commands (e.g., ls | grep .txt)
#define LIST 4   // List of commands (e.g., ls ; pwd)
#define BACK 5   // Background execution (e.g., sleep 10 &)

/* Maximum number of arguments allowed in a command */
#define MAXARGS 100

/* Global variable to store the exit status of the last command */
extern int g_last_exit_status;

/*
 * Base command structure
 * All other command structures start with this to allow type-based casting
 */
typedef struct s_cmd
{
    int type;  // Identifies the type of command (EXEC, REDIR, etc.)
}   t_cmd;

/*
 * Structure for simple command execution
 * Example: ls -l /home
 */
typedef struct s_execcmd
{
    int type;                  // Must be EXEC
    char *av[MAXARGS];        // Array of argument strings (av[0] is the command)
    char *eav[MAXARGS];       // Array of pointers to end of each argument string
}   t_execcmd;

/*
 * Structure for input/output redirection
 * Examples: ls > output.txt, cat < input.txt
 */
typedef struct s_redircmd
{
    int type;              // Must be REDIR
    struct s_cmd *cmd;     // The command being redirected
    char *file;           // The file to redirect to/from
    char *efile;          // Pointer to end of file string
    int mode;            // File mode (O_RDONLY, O_WRONLY, etc.)
    int fd;             // File descriptor to redirect (0 for input, 1 for output)
}   t_redircmd;

/*
 * Structure for command pipelines
 * Example: ls -l | grep .txt | wc -l
 */
typedef struct s_pipecmd
{
    int type;              // Must be PIPE
    struct s_cmd *left;    // Command before the pipe
    struct s_cmd *right;   // Command after the pipe
}   t_pipecmd;

/*
 * Structure for command lists (commands separated by semicolon)
 * Example: ls ; pwd ; echo hello
 */
typedef struct s_listcmd
{
    int type;              // Must be LIST
    struct s_cmd *left;    // First command in sequence
    struct s_cmd *right;   // Next command in sequence
}   t_listcmd;

/*
 * Structure for background commands
 * Example: sleep 100 &
 */
typedef struct s_backcmd
{
    int type;              // Must be BACK
    struct s_cmd *cmd;     // The command to run in background
}   t_backcmd;

/* Basic shell functions */
char *readline_helper(void);              // Read a line from input with prompt
void *get_cwd(char *buf, size_t size);   // Get current working directory
void wtf(void);                          // Error handler
int forkk(void);                         // Fork wrapper with error checking
void runcmd(struct s_cmd *cmd);          // Execute a command structure
struct s_cmd *tokenize(const char *line); // Convert input line to command structure
struct s_cmd* nulterm(struct s_cmd *cmd); // Ensure proper string termination

/* Parser and command constructor functions */
struct s_cmd *parseexec(char **input_ptr, char *input_end);     // Parse a simple command
int peek(char **input_ptr, char *input_end, char *toks);        // Look ahead for tokens
int gettoken(char **input_ptr, char *input_end,                 // Get next token
            char **token_start, char **token_end);
struct s_cmd *pipecmd(struct s_cmd *left, struct s_cmd *right); // Create a pipe command
struct s_cmd *backcmd(struct s_cmd *subcmd);                    // Create a background command
struct s_cmd *listcmd(struct s_cmd *left, struct s_cmd *right); // Create a command list
struct s_cmd *redircmd(struct s_cmd *subcmd, char *file,        // Create a redirection
                      char *efile, int mode, int fd);
struct s_cmd *execcmd(void);                                    // Create a simple command
struct s_cmd *parse_redirs(struct s_cmd *cmd,                   // Parse redirections
                          char **input_ptr, char *input_end);
struct s_cmd *parse_block(char **input_ptr, char *input_end);   // Parse parenthesized block
struct s_cmd *parse_line(char **input_ptr, char *input_end);    // Parse entire command line

/* Built-in command handling */
int is_builtin(char *cmd);                                      // Check if command is built-in
int handle_builtin(char **argv);                                // Execute built-in command
char *expand_variables(const char *str, size_t len);            // Expand environment variables
void set_exit_status(int status);                              // Set last command exit status

#endif