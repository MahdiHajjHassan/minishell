#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Added for malloc and free

// Simulate the functions
int remove_exec_quotes(char **q, char **eq)
{
    int quote_type = 0;
    if (**q == '"' && *(*eq - 1) == '"')
    {
        (*q)++;
        (*eq)--;
        quote_type = '"';
    }
    else if (**q == '\'' && *(*eq - 1) == '\'')
    {
        (*q)++;
        (*eq)--;
        quote_type = '\'';
    }
    return (quote_type);
}

char *process_escaped(char *q, size_t len)
{
    char *result = malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, q, len);
    result[len] = '\0';
    return result;
}

char *expand_variables(char *str, size_t len, char **env_copy)
{
    // For now, just return a copy of the string
    char *result = malloc(len + 1);
    if (!result) return NULL;
    strcpy(result, str);
    return result;
}

char *process_argument_with_expansion(char *q, char *eq, char **env_copy, int quote_type)
{
    size_t len = eq - q;
    char *processed = process_escaped(q, len);
    if (!processed) return NULL;
    
    if (quote_type != '\'')
    {
        char *expanded = expand_variables(processed, strlen(processed), env_copy);
        free(processed);
        if (!expanded) return NULL;
        return expanded;
    }
    return processed;
}

int is_builtin(char *cmd)
{
    if (!strcmp(cmd, "echo")) return 1;
    if (!strcmp(cmd, "cd")) return 1;
    if (!strcmp(cmd, "pwd")) return 1;
    if (!strcmp(cmd, "export")) return 1;
    if (!strcmp(cmd, "unset")) return 1;
    if (!strcmp(cmd, "env")) return 1;
    if (!strcmp(cmd, "exit")) return 1;
    return 0;
}

int main()
{
    char *env_copy[] = {"PATH=/usr/bin", NULL};
    
    // Test case 1: "echo" hello
    printf("=== Test 1: \"echo\" hello ===\n");
    char *cmd1 = "\"echo\"";
    char *q1 = cmd1;
    char *eq1 = cmd1 + strlen(cmd1);
    
    int quote_type1 = remove_exec_quotes(&q1, &eq1);
    printf("Quote type: %d\n", quote_type1);
    printf("After quote removal: '%.*s'\n", (int)(eq1 - q1), q1);
    
    char *processed1 = process_argument_with_expansion(q1, eq1, env_copy, quote_type1);
    printf("Processed: '%s'\n", processed1);
    printf("Is builtin: %d\n", is_builtin(processed1));
    free(processed1);
    
    // Test case 2: echo "hello"
    printf("\n=== Test 2: echo \"hello\" ===\n");
    char *arg2 = "\"hello\"";
    char *q2 = arg2;
    char *eq2 = arg2 + strlen(arg2);
    
    int quote_type2 = remove_exec_quotes(&q2, &eq2);
    printf("Quote type: %d\n", quote_type2);
    printf("After quote removal: '%.*s'\n", (int)(eq2 - q2), q2);
    
    char *processed2 = process_argument_with_expansion(q2, eq2, env_copy, quote_type2);
    printf("Processed: '%s'\n", processed2);
    free(processed2);
    
    return 0;
} 