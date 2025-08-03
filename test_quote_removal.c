#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Test the exact quote removal logic from the shell
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
    return quote_type;
}

// Simulate process_escaped
char *process_escaped(const char *input, size_t len)
{
    char *output = malloc(len + 1);
    if (!output)
        return NULL;
    size_t i = 0;
    while (i < len)
    {
        output[i] = input[i];
        i++;
    }
    output[i] = '\0';
    return output;
}

// Test case
void test_argument_processing(const char *token)
{
    printf("\nTesting token: '%s'\n", token);
    
    // Simulate the parsing process
    char *q = (char *)token;
    char *eq = (char *)token + strlen(token);
    
    printf("Before remove_exec_quotes: q='%.*s', len=%ld\n", 
           (int)(eq - q), q, eq - q);
    
    // Remove quotes
    int quote_type = remove_exec_quotes(&q, &eq);
    
    printf("After remove_exec_quotes: q='%.*s', len=%ld, quote_type=%c\n", 
           (int)(eq - q), q, eq - q, quote_type ? quote_type : '0');
    
    // Process escaped (just copies the string)
    size_t len = eq - q;
    char *processed = process_escaped(q, len);
    
    printf("After process_escaped: '%s', len=%zu\n", processed, strlen(processed));
    
    // This is what would be stored in av[]
    printf("Would store in av[]: '%s'\n", processed);
    
    // Check if it's "echo"
    int is_echo = strcmp(processed, "echo") == 0;
    printf("Is 'echo'? %s (strcmp result: %d)\n", 
           is_echo ? "YES" : "NO", strcmp(processed, "echo"));
    
    free(processed);
}

int main()
{
    // Test different cases
    test_argument_processing("echo");
    test_argument_processing("\"echo\"");
    test_argument_processing("'echo'");
    test_argument_processing("hello");
    test_argument_processing("\"hello\"");
    test_argument_processing("\"\"");
    test_argument_processing("\"hello world\"");
    
    return 0;
}