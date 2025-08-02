#include <stdio.h>
#include <string.h>

// Simulate the remove_exec_quotes function
int remove_exec_quotes(char **q, char **eq)
{
    int quote_type;

    quote_type = 0;
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

int main()
{
    char *test1 = "\"echo\"";
    char *q1 = test1;
    char *eq1 = test1 + strlen(test1);
    
    char *test2 = "\"hello\"";
    char *q2 = test2;
    char *eq2 = test2 + strlen(test2);
    
    printf("Before quote removal:\n");
    printf("test1: '%s', q1: '%c', eq1: '%c'\n", test1, *q1, *(eq1-1));
    printf("test2: '%s', q2: '%c', eq2: '%c'\n", test2, *q2, *(eq2-1));
    
    int quote_type1 = remove_exec_quotes(&q1, &eq1);
    int quote_type2 = remove_exec_quotes(&q2, &eq2);
    
    printf("\nAfter quote removal:\n");
    printf("test1 quote_type: %d, q1: '%c', eq1: '%c'\n", quote_type1, *q1, *(eq1-1));
    printf("test2 quote_type: %d, q2: '%c', eq2: '%c'\n", quote_type2, *q2, *(eq2-1));
    
    printf("\nExtracted strings:\n");
    printf("test1: '%.*s'\n", (int)(eq1 - q1), q1);
    printf("test2: '%.*s'\n", (int)(eq2 - q2), q2);
    
    return 0;
} 