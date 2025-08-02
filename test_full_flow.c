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

char *skip_whitespace(char *s, char *input_end, char *space)
{
    while (s < input_end && strchr(space, *s))
        s++;
    return s;
}

char *handle_default_token(char *s, char *input_end, char *input_ptr, char *symbols, char *space)
{
    char quote = 0;
    
    while (s < input_end)
    {
        if (!quote)
        {
            if (strchr(symbols, *s) || strchr(space, *s))
                break;
            if (*s == '"' || *s == '\'')
                quote = *s;
        }
        else if (quote && *s == quote)
        {
            quote = 0;
        }
        s++;
    }
    
    if (quote)
        return NULL;
    
    return s;
}

int gettoken(char **input_ptr, char *input_end, char **token_start, char **token_end)
{
    char *s;
    int ret;
    char space[6];
    char symbols[8];
    
    init_space_array(space);
    init_symbols_array(symbols);
    
    s = *input_ptr;
    while (s < input_end && strchr(space, *s))
        s++;
    
    if (token_start)
        *token_start = s;
    
    if (*s == 0)
        return 0;
    
    // Handle special characters
    if (strchr(symbols, *s))
    {
        ret = *s;
        s++;
        if (token_end)
            *token_end = s;
        s = skip_whitespace(s, input_end, space);
        *input_ptr = s;
        return ret;
    }
    
    // Handle default tokens
    ret = 'a';
    char *new_s = handle_default_token(s, input_end, s, symbols, space);
    if (!new_s)
        return 0;
    
    if (token_end)
        *token_end = new_s;
    
    new_s = skip_whitespace(new_s, input_end, space);
    *input_ptr = new_s;
    return ret;
}

int main()
{
    // Test case 1: echo "hello"
    printf("=== Test 1: echo \"hello\" ===\n");
    char *input1 = "echo \"hello\"";
    char *input_ptr1 = input1;
    char *input_end1 = input1 + strlen(input1);
    
    int token_count = 0;
    while (input_ptr1 < input_end1)
    {
        char *q, *eq;
        int tok = gettoken(&input_ptr1, input_end1, &q, &eq);
        
        if (tok == 0)
            break;
            
        printf("Token %d: type=%d, '%.*s'\n", token_count++, tok, (int)(eq - q), q);
        
        if (input_ptr1 >= input_end1)
            break;
    }
    
    // Test case 2: "echo" hello
    printf("\n=== Test 2: \"echo\" hello ===\n");
    char *input2 = "\"echo\" hello";
    char *input_ptr2 = input2;
    char *input_end2 = input2 + strlen(input2);
    
    token_count = 0;
    while (input_ptr2 < input_end2)
    {
        char *q, *eq;
        int tok = gettoken(&input_ptr2, input_end2, &q, &eq);
        
        if (tok == 0)
            break;
            
        printf("Token %d: type=%d, '%.*s'\n", token_count++, tok, (int)(eq - q), q);
        
        if (input_ptr2 >= input_end2)
            break;
    }
    
    return 0;
} 