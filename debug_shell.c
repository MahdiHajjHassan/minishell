#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Debug function to print command structure
void debug_print_cmd(char **av)
{
    printf("[DEBUG] Command structure:\n");
    for (int i = 0; av[i] != NULL; i++)
    {
        printf("  av[%d] = '%s' (length: %zu)\n", i, av[i], strlen(av[i]));
    }
}

// Simulate is_builtin check
int is_builtin_debug(char *cmd)
{
    printf("[DEBUG] Checking if '%s' is builtin\n", cmd);
    printf("  strcmp(cmd, \"echo\") = %d\n", strcmp(cmd, "echo"));
    
    if (!strcmp(cmd, "echo"))
        return 1;
    return 0;
}

// Test quote removal on command
void test_command_parsing(const char *input)
{
    printf("\n=== Testing: %s ===\n", input);
    
    // Simulate tokenization and quote removal
    char *copy = strdup(input);
    char *token = strtok(copy, " ");
    
    if (token)
    {
        printf("First token: '%s'\n", token);
        
        // Simulate quote removal
        char *q = token;
        char *eq = token + strlen(token);
        
        if (*q == '"' && *(eq-1) == '"')
        {
            q++;
            eq--;
            char *unquoted = malloc(eq - q + 1);
            strncpy(unquoted, q, eq - q);
            unquoted[eq - q] = '\0';
            printf("After quote removal: '%s'\n", unquoted);
            
            // Check if it's a builtin
            is_builtin_debug(unquoted);
            
            free(unquoted);
        }
        else
        {
            printf("No quotes to remove\n");
            is_builtin_debug(token);
        }
    }
    
    free(copy);
}

int main()
{
    test_command_parsing("echo hello");
    test_command_parsing("\"echo\" hello");
    test_command_parsing("'echo' hello");
    
    return 0;
}