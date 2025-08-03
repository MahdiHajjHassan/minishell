#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simulate the argument processing flow
void simulate_argument_processing(const char *arg) {
    printf("\n=== Processing: %s ===\n", arg);
    
    // Step 1: Check if entire argument is quoted
    size_t len = strlen(arg);
    if (len >= 2 && 
        ((arg[0] == '"' && arg[len-1] == '"') ||
         (arg[0] == '\'' && arg[len-1] == '\''))) {
        printf("1. Entire argument is quoted\n");
        printf("   Would remove outer quotes: '%.*s'\n", (int)(len-2), arg+1);
    } else {
        printf("1. Argument is not entirely quoted\n");
        printf("   Would process as-is: '%s'\n", arg);
    }
    
    // Step 2: Count quote pairs
    int quote_count = 0;
    const char *ptr = arg;
    while (*ptr) {
        if (*ptr == '"' || *ptr == '\'') {
            char quote = *ptr;
            ptr++;
            while (*ptr && *ptr != quote) ptr++;
            if (*ptr == quote) {
                quote_count++;
                ptr++;
            }
        } else {
            ptr++;
        }
    }
    printf("2. Quote pairs found: %d\n", quote_count);
    
    // Step 3: Determine processing path
    if (quote_count > 1) {
        printf("3. Would use concatenate_quoted_strings\n");
    } else {
        printf("3. Would use process_single_argument\n");
    }
    
    // Step 4: Simulate what export would receive
    printf("4. Export would process: '%s'\n", arg);
    
    // Find equals sign
    const char *equals = strchr(arg, '=');
    if (equals) {
        printf("   Variable name: '%.*s'\n", (int)(equals - arg), arg);
        printf("   Variable value: '%s'\n", equals + 1);
        
        // Check if value has quotes
        const char *value = equals + 1;
        size_t vlen = strlen(value);
        if (vlen >= 2 && 
            ((value[0] == '"' && value[vlen-1] == '"') ||
             (value[0] == '\'' && value[vlen-1] == '\''))) {
            printf("   Value has quotes, would remove them: '%.*s'\n", 
                   (int)(vlen-2), value+1);
        }
    }
}

int main() {
    simulate_argument_processing("TEST=hello");
    simulate_argument_processing("TEST=\"hello world\"");
    simulate_argument_processing("TEST='hello world'");
    simulate_argument_processing("\"TEST=hello world\"");
    simulate_argument_processing("TEST=\"hello\" WORLD=\"world\"");
    
    return 0;
}