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

/* Structure to hold redirection parameters */
typedef struct s_redir_params
{
    int mode;  /* File mode (O_RDONLY, O_WRONLY, etc.) */
    int fd;    /* File descriptor to redirect (0 for input, 1 for output) */
}   t_redir_params;

/* Structure to hold process arguments parameters */
typedef struct s_process_args_params
{
    struct s_execcmd	*cmd;
    char				**input_ptr;
    char				*input_end;
    int					*argc;
}   t_process_args_params;

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

/* Main helper functions */
void display_prompt(void);               // Display shell prompt
void init_signals(void);                 // Initialize signal state
int handle_line_input(char **line);      // Handle line input from user
int handle_tokenize(char *line, struct s_cmd **cmd); // Handle command tokenization
void expand_builtin_args(struct s_execcmd *ecmd); // Expand variables in builtin args
int handle_builtin_cmd(struct s_cmd *cmd, char *line); // Handle builtin command execution
void handle_child_status(int status);    // Handle child process status
void execute_cmd(struct s_cmd *cmd);     // Execute external command

/* Parser and command constructor functions */
struct s_cmd *parseexec(char **input_ptr, char *input_end);     // Parse a simple command
int peek(char **input_ptr, char *input_end, char *toks);        // Look ahead for tokens
int gettoken(char **input_ptr, char *input_end,                 // Get next token
            char **token_start, char **token_end);
struct s_cmd *pipecmd(struct s_cmd *left, struct s_cmd *right); // Create a pipe command
struct s_cmd *backcmd(struct s_cmd *subcmd);                    // Create a background command
struct s_cmd *listcmd(struct s_cmd *left, struct s_cmd *right); // Create a command list
struct s_cmd *redircmd(struct s_cmd *subcmd, char *file,        // Create a redirection
                      char *efile, t_redir_params params);
struct s_cmd *execcmd(void);                                    // Create a simple command
struct s_cmd *parse_redirs(struct s_cmd *cmd,                   // Parse redirections
                          char **input_ptr, char *input_end);
struct s_cmd *parse_block(char **input_ptr, char *input_end);   // Parse parenthesized block
struct s_cmd *parse_line(char **input_ptr, char *input_end);    // Parse entire command line

/* Parser helper functions */
char *init_output_buffer(size_t len);
char get_escape_char(char c);
void handle_escape_sequence(const char *input, size_t *i, char *output, size_t *j);
char *process_escaped(const char *input, size_t len);
int get_redir_token(char **input_ptr, char *input_end, char **q, char **eq);
void remove_redir_quotes(char **q, char **eq);
char *process_filename(char *q, char *eq);
struct s_cmd *apply_input_redir(struct s_cmd *cmd, char *file);
struct s_cmd *apply_output_redir(struct s_cmd *cmd, char *file);
struct s_cmd *apply_append_redir(struct s_cmd *cmd, char *file);
struct s_cmd *handle_redir_token(struct s_cmd *cmd, int tok, char *file);
struct s_cmd *init_exec_cmd(void);
int get_exec_token(char **input_ptr, char *input_end, char **q, char **eq);
void remove_exec_quotes(char **q, char **eq);
char *process_argument(char *q, char *eq);
void add_argument(struct s_execcmd *cmd, char *processed, int *argc);
void finalize_exec_cmd(struct s_execcmd *cmd, int argc);
struct s_cmd *process_arguments(struct s_cmd *ret, t_process_args_params params);

/* Built-in command handling */
int is_builtin(char *cmd);                                      // Check if command is built-in
int handle_builtin(char **argv);                                // Execute built-in command
char *expand_variables(const char *str, size_t len);            // Expand environment variables
void set_exit_status(int status);                              // Set last command exit status

/* Builtin helper functions */
int cd_to_home(void);                                           // Change directory to home
int cd_to_path(char *path);                                     // Change directory to specified path
int parse_export_arg(char *arg, char **name, char **value);     // Parse export argument
void remove_quotes(char **value);                               // Remove quotes from value
int set_environment_var(char *name, char *value);               // Set environment variable

/* Builtin command functions */
int builtin_echo(char **argv);                                  // Echo command implementation
int builtin_cd(char **argv);                                    // Cd command implementation
int builtin_pwd(char **argv);                                   // Pwd command implementation
int builtin_exit(char **argv);                                  // Exit command implementation

#endif