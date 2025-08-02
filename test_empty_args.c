#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_echo_with_args(char **argv)
{
    printf("\nTesting echo with arguments:\n");
    for (int i = 0; argv[i] != NULL; i++)
    {
        printf("  argv[%d] = '%s' (length: %zu)\n", i, argv[i], strlen(argv[i]));
    }
    
    // Simulate echo builtin
    printf("Echo output: ");
    int i = 1; // Skip argv[0] which is "echo"
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
}

int main()
{
    // Test case 1: echo hello
    char *test1[] = {"echo", "hello", NULL};
    test_echo_with_args(test1);
    
    // Test case 2: echo "hello" (after quote removal)
    char *test2[] = {"echo", "hello", NULL};
    test_echo_with_args(test2);
    
    // Test case 3: echo "" (empty string after quote removal)
    char *test3[] = {"echo", "", NULL};
    test_echo_with_args(test3);
    
    // Test case 4: echo with no arguments
    char *test4[] = {"echo", NULL};
    test_echo_with_args(test4);
    
    return 0;
}