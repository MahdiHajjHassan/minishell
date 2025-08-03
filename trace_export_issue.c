#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Test the quote counting logic
void test_count_quotes(const char *str) {
    printf("\nTesting: '%s'\n", str);
    
    char *s = strdup(str);
    char *ptr = s;
    char *end = s + strlen(s);
    int count = 0;
    
    while (ptr < end) {
        if (*ptr == '"' || *ptr == '\'') {
            char quote = *ptr;
            printf("  Found %c at position %ld\n", quote, ptr - s);
            ptr++; // Skip opening quote
            
            // Find closing quote
            while (ptr < end && *ptr != quote) {
                ptr++;
            }
            
            if (ptr < end && *ptr == quote) {
                printf("  Found closing %c at position %ld\n", quote, ptr - s);
                ptr++; // Skip closing quote
            } else {
                printf("  No closing quote found!\n");
            }
            count++;
        } else {
            ptr++;
        }
    }
    
    printf("  Total quote pairs: %d\n", count);
    free(s);
}

// Test remove_exec_quotes logic
void test_remove_quotes(const char *str) {
    printf("\nTesting remove_exec_quotes on: '%s'\n", str);
    
    char *s = strdup(str);
    char *q = s;
    char *eq = s + strlen(s);
    
    printf("  First char: '%c', Last char: '%c'\n", *q, *(eq-1));
    
    if (*q == '"' && *(eq - 1) == '"') {
        printf("  Would remove double quotes\n");
        q++;
        eq--;
        printf("  Result: '%.*s'\n", (int)(eq - q), q);
    } else if (*q == '\'' && *(eq - 1) == '\'') {
        printf("  Would remove single quotes\n");
        q++;
        eq--;
        printf("  Result: '%.*s'\n", (int)(eq - q), q);
    } else {
        printf("  No quotes to remove\n");
        printf("  Result: '%s'\n", s);
    }
    
    free(s);
}

int main() {
    // Test quote counting
    test_count_quotes("TEST=hello");
    test_count_quotes("TEST=\"hello world\"");
    test_count_quotes("TEST='hello world'");
    test_count_quotes("\"TEST=hello world\"");
    test_count_quotes("TEST=\"hello\" ANOTHER=\"world\"");
    
    // Test quote removal
    test_remove_quotes("TEST=hello");
    test_remove_quotes("TEST=\"hello world\"");
    test_remove_quotes("\"TEST=hello world\"");
    test_remove_quotes("'hello world'");
    
    return 0;
}