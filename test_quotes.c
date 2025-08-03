#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulating the remove_exec_quotes function
int remove_exec_quotes(char **q, char **eq)
{
    int quote_type = 0;
    
    printf("Before remove_exec_quotes:\n");
    printf("  q points to: '%.*s'\n", (int)(*eq - *q), *q);
    printf("  *q = '%c', *(*eq-1) = '%c'\n", **q, *(*eq - 1));
    
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
    
    printf("After remove_exec_quotes:\n");
    printf("  q points to: '%.*s'\n", (int)(*eq - *q), *q);
    printf("  quote_type = %c\n", quote_type ? quote_type : '0');
    
    return quote_type;
}

void test_case(const char *test_name, char *input)
{
    printf("\n=== Test: %s ===\n", test_name);
    printf("Input: '%s'\n", input);
    
    char *q = input;
    char *eq = input + strlen(input);
    
    int quote_type = remove_exec_quotes(&q, &eq);
    
    // Create the resulting string
    size_t len = eq - q;
    char *result = malloc(len + 1);
    strncpy(result, q, len);
    result[len] = '\0';
    
    printf("Result: '%s' (length: %zu)\n", result, len);
    
    free(result);
}

int main()
{
    // Test cases that were problematic
    test_case("Quoted command", "\"echo\"");
    test_case("Quoted argument", "\"hello\"");
    test_case("Unquoted command", "echo");
    test_case("Unquoted argument", "hello");
    test_case("Single quoted command", "'echo'");
    test_case("Empty quotes", "\"\"");
    test_case("Nested quotes", "\"'hello'\"");
    
    return 0;
}