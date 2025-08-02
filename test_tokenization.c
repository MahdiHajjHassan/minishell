#include <stdio.h>
#include <string.h>

// Simulate the tokenizer functions
void init_space_array(char *space)
{
    space[0] = ' ';
    space[1] = '\t';
    space[2] = '\r';
    space[3] = '\n';
    space[4] = '\v';
    space[5] = '\0';
}

void init_symbols_array(char *symbols)
{
    symbols[0] = '<';
    symbols[1] = '>';
    symbols[2] = '|';
    symbols[3] = '&';
    symbols[4] = '(';
    symbols[5] = ')';
    symbols[6] = '\0';
}

char *handle_default_token(char *s, char *input_end, char *input_ptr, char *symbols, char *space)
{
    char quote = 0;
    
    printf("DEBUG: handle_default_token called with s='%c', input_end='%c'\n", *s, *(input_end-1));
    
    while (s < input_end)
    {
        printf("DEBUG: Processing char '%c', quote=%d\n", *s, quote);
        
        if (!quote)
        {
            if (strchr(symbols, *s) || strchr(space, *s))
            {
                printf("DEBUG: Found separator '%c', breaking\n", *s);
                break;
            }
            if (*s == '"' || *s == '\'')
            {
                printf("DEBUG: Found quote '%c'\n", *s);
                quote = *s;
            }
        }
        else if (quote && *s == quote)
        {
            printf("DEBUG: Found closing quote '%c'\n", *s);
            quote = 0;
        }
        s++;
    }
    
    if (quote)
    {
        printf("DEBUG: Unclosed quote, returning NULL\n");
        return NULL;
    }
    
    printf("DEBUG: Token ends at '%c'\n", *s);
    return s;
}

int main()
{
    char space[6];
    char symbols[8];
    
    init_space_array(space);
    init_symbols_array(symbols);
    
    printf("Space chars: '%s'\n", space);
    printf("Symbol chars: '%s'\n", symbols);
    
    // Test case 1: echo "hello"
    printf("\n=== Test 1: echo \"hello\" ===\n");
    char *test1 = "echo \"hello\"";
    char *end1 = test1 + strlen(test1);
    
    char *result1 = handle_default_token(test1, end1, test1, symbols, space);
    if (result1)
        printf("Result: '%.*s'\n", (int)(result1 - test1), test1);
    else
        printf("Result: NULL\n");
    
    // Test case 2: "echo" hello
    printf("\n=== Test 2: \"echo\" hello ===\n");
    char *test2 = "\"echo\" hello";
    char *end2 = test2 + strlen(test2);
    
    char *result2 = handle_default_token(test2, end2, test2, symbols, space);
    if (result2)
        printf("Result: '%.*s'\n", (int)(result2 - test2), test2);
    else
        printf("Result: NULL\n");
    
    return 0;
} 