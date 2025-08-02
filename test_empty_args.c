#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulate the exact functions
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
    printf("DEBUG: process_escaped called with len=%zu\n", len);
    char *result = malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, q, len);
    result[len] = '\0';
    printf("DEBUG: process_escaped result: '%s'\n", result);
    return result;
}

char *expand_variables(char *str, size_t len, char **env_copy)
{
    printf("DEBUG: expand_variables called with str='%s', len=%zu\n", str, len);
    // For now, just return a copy of the string
    char *result = malloc(len + 1);
    if (!result) return NULL;
    strcpy(result, str);
    printf("DEBUG: expand_variables result: '%s'\n", result);
    return result;
}

char *process_argument_with_expansion(char *q, char *eq, char **env_copy, int quote_type)
{
    size_t len = eq - q;
    printf("DEBUG: process_argument_with_expansion: len=%zu, quote_type=%d\n", len, quote_type);
    
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

int main()
{
    char *env_copy[] = {"PATH=/usr/bin", NULL};
    
    // Test case 1: Empty quoted string
    printf("=== Test 1: Empty quoted string ===\n");
    char *cmd1 = "\"\"";
    char *q1 = cmd1;
    char *eq1 = cmd1 + strlen(cmd1);
    
    printf("Original: '%s'\n", cmd1);
    int quote_type1 = remove_exec_quotes(&q1, &eq1);
    printf("After quote removal: quote_type=%d, len=%zu\n", quote_type1, eq1 - q1);
    
    char *processed1 = process_argument_with_expansion(q1, eq1, env_copy, quote_type1);
    printf("Final result: '%s'\n", processed1);
    if (processed1) free(processed1);
    
    // Test case 2: Single character
    printf("\n=== Test 2: Single character ===\n");
    char *cmd2 = "\"a\"";
    char *q2 = cmd2;
    char *eq2 = cmd2 + strlen(cmd2);
    
    printf("Original: '%s'\n", cmd2);
    int quote_type2 = remove_exec_quotes(&q2, &eq2);
    printf("After quote removal: quote_type=%d, len=%zu\n", quote_type2, eq2 - q2);
    
    char *processed2 = process_argument_with_expansion(q2, eq2, env_copy, quote_type2);
    printf("Final result: '%s'\n", processed2);
    if (processed2) free(processed2);
    
    return 0;
} 