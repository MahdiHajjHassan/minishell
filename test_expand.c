#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simulate what expand_variables might do
char *expand_variables_simple(const char *str)
{
    // If the string has no $ signs, it should just return a copy
    if (!strchr(str, '$'))
        return strdup(str);
    
    // Otherwise do expansion...
    return strdup(str); // Simplified
}

// Test what happens with different inputs
void test_expansion(const char *input)
{
    printf("Testing: '%s'\n", input);
    
    // Simulate the flow:
    // 1. After quote removal, we have the string
    // 2. It goes through expand_variables
    
    char *expanded = expand_variables_simple(input);
    printf("  After expansion: '%s' (len=%zu)\n", expanded, strlen(expanded));
    
    free(expanded);
}

int main()
{
    test_expansion("echo");
    test_expansion("hello");
    test_expansion("");
    test_expansion("hello world");
    test_expansion("$HOME");
    test_expansion("hello$USER");
    
    return 0;
}