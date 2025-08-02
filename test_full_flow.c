#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulate the full flow from tokenization to execution

// Step 1: Tokenization (gettoken returns token with quotes)
void simulate_tokenization(const char *input, char **tokens, int *token_count)
{
    printf("=== Tokenization of: %s ===\n", input);
    
    char *copy = strdup(input);
    char *ptr = copy;
    *token_count = 0;
    
    while (*ptr && *token_count < 10)
    {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        char quote = 0;
        
        // Find token end (respecting quotes)
        while (*ptr)
        {
            if (!quote)
            {
                if (*ptr == ' ' || *ptr == '\t') break;
                if (*ptr == '"' || *ptr == '\'') quote = *ptr;
            }
            else if (*ptr == quote)
            {
                quote = 0;
            }
            ptr++;
        }
        
        // Extract token
        size_t len = ptr - start;
        tokens[*token_count] = malloc(len + 1);
        strncpy(tokens[*token_count], start, len);
        tokens[*token_count][len] = '\0';
        
        printf("  Token %d: '%s'\n", *token_count, tokens[*token_count]);
        (*token_count)++;
    }
    
    free(copy);
}

// Step 2: Process arguments (remove quotes and store in av array)
void process_arguments(char **tokens, int token_count, char **av)
{
    printf("\n=== Processing arguments ===\n");
    
    for (int i = 0; i < token_count; i++)
    {
        char *q = tokens[i];
        char *eq = tokens[i] + strlen(tokens[i]);
        
        printf("  Processing token %d: '%s'\n", i, tokens[i]);
        
        // Remove quotes if present
        if (*q == '"' && *(eq-1) == '"')
        {
            q++;
            eq--;
            printf("    Removed double quotes\n");
        }
        else if (*q == '\'' && *(eq-1) == '\'')
        {
            q++;
            eq--;
            printf("    Removed single quotes\n");
        }
        
        // Copy processed argument
        size_t len = eq - q;
        av[i] = malloc(len + 1);
        strncpy(av[i], q, len);
        av[i][len] = '\0';
        
        printf("    Result: av[%d] = '%s' (length: %zu)\n", i, av[i], strlen(av[i]));
    }
    av[token_count] = NULL;
}

// Step 3: Check if builtin
int is_builtin(char *cmd)
{
    printf("\n=== Builtin check ===\n");
    printf("  Checking if '%s' is builtin\n", cmd);
    printf("  strcmp(cmd, \"echo\") = %d\n", strcmp(cmd, "echo"));
    
    return strcmp(cmd, "echo") == 0;
}

// Step 4: Execute
void execute_command(char **av)
{
    printf("\n=== Execution ===\n");
    
    if (!av[0])
    {
        printf("  No command\n");
        return;
    }
    
    if (is_builtin(av[0]))
    {
        printf("  Executing builtin echo:\n");
        printf("  Output: ");
        
        // Simulate echo builtin
        int i = 1;
        while (av[i])
        {
            printf("%s", av[i]);
            if (av[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("  Would execute external command: %s\n", av[0]);
    }
}

void test_full_flow(const char *input)
{
    printf("\n\n========== FULL FLOW TEST ==========\n");
    printf("Input: %s\n", input);
    
    char *tokens[10];
    int token_count;
    char *av[10];
    
    // Step 1: Tokenize
    simulate_tokenization(input, tokens, &token_count);
    
    // Step 2: Process arguments
    process_arguments(tokens, token_count, av);
    
    // Step 3 & 4: Check builtin and execute
    execute_command(av);
    
    // Cleanup
    for (int i = 0; i < token_count; i++)
    {
        free(tokens[i]);
        free(av[i]);
    }
}

int main()
{
    // Test the problematic cases
    test_full_flow("echo hello");
    test_full_flow("\"echo\" hello");
    test_full_flow("echo \"hello\"");
    test_full_flow("echo \"\"");
    
    return 0;
}