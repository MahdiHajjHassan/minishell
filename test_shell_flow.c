#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulating the tokenizer behavior
void simulate_tokenizer(const char *input)
{
    printf("\n=== Tokenizing: %s ===\n", input);
    
    // Simple tokenizer that respects quotes
    char *copy = strdup(input);
    char *ptr = copy;
    int token_num = 0;
    
    while (*ptr)
    {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        char quote = 0;
        
        // Find token end
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
        char saved = *ptr;
        *ptr = '\0';
        printf("Token %d: '%s'\n", token_num++, start);
        
        // Simulate quote removal
        char *q = start;
        char *eq = start + strlen(start);
        if (*q == '"' && *(eq-1) == '"')
        {
            q++;
            eq--;
            printf("  After quote removal: '%.*s'\n", (int)(eq - q), q);
        }
        else if (*q == '\'' && *(eq-1) == '\'')
        {
            q++;
            eq--;
            printf("  After quote removal: '%.*s'\n", (int)(eq - q), q);
        }
        else
        {
            printf("  No quotes to remove\n");
        }
        
        *ptr = saved;
    }
    
    free(copy);
}

int main()
{
    // Test cases from the user's problem
    printf("Testing problematic cases:\n");
    simulate_tokenizer("\"echo\" hello");
    simulate_tokenizer("echo \"hello\"");
    simulate_tokenizer("echo hello");
    simulate_tokenizer("\"echo\"");
    simulate_tokenizer("echo \"\"");
    simulate_tokenizer("\"echo\" \"hello\" \"world\"");
    
    return 0;
}