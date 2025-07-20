#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Global variable to store last command's exit status */
int g_last_exit_status = 0;

/*
 * set_exit_status - Update the last command's exit status
 * 
 * Extracts the actual exit status from the wait status value
 * and stores it in the global variable.
 * 
 * @status: Status value from wait() system call
 */
void set_exit_status(int status)
{
    g_last_exit_status = status;
}

/*
 * get_var_name_len - Get length of environment variable name
 * 
 * Determines the length of a variable name in a string.
 * Handles special case of $? for exit status.
 * Valid variable names contain alphanumeric chars and underscore.
 * 
 * @str: String starting with variable name
 * 
 * Returns: Length of variable name
 */
static size_t get_var_name_len(const char *str)
{
    size_t len = 0;
    
    if (*str == '?') // Special case for $?
        return 1;
        
    while (str[len] && (isalnum(str[len]) || str[len] == '_'))
        len++;
    return len;
}

/*
 * get_env_value - Get value of environment variable
 * 
 * Retrieves value of environment variable or exit status.
 * Handles special $? variable for last command's exit status.
 * 
 * @name: Name of variable
 * @name_len: Length of variable name
 * 
 * Returns:
 *   - Newly allocated string with variable value
 *   - Empty string if variable not found
 *   - NULL on memory allocation failure
 */
static char *get_env_value(const char *name, size_t name_len)
{
    char *temp;
    char *value;
    
    if (name_len == 1 && *name == '?')
    {
        // Handle $? special case
        char num[32];
        snprintf(num, sizeof(num), "%d", g_last_exit_status);
        return strdup(num);
    }
    
    // Create null-terminated copy of variable name
    temp = malloc(name_len + 1);
    if (!temp)
        return NULL;
    strncpy(temp, name, name_len);
    temp[name_len] = '\0';
    
    value = getenv(temp);
    free(temp);
    
    return value ? strdup(value) : strdup("");
}

/*
 * expand_variables - Expand environment variables in string
 * 
 * Replaces all environment variables ($NAME) with their values.
 * Also handles the special $? variable for exit status.
 * 
 * Example:
 *   Input:  "Hello $USER, status=$?"
 *   Output: "Hello john, status=0"
 * 
 * @str: Input string containing variables to expand
 * @len: Length of input string
 * 
 * Returns:
 *   - New string with variables expanded
 *   - NULL on memory allocation failure
 */
char *expand_variables(const char *str, size_t len)
{
    char *result;
    size_t i = 0;
    size_t j = 0;
    size_t alloc_size = len * 2; // Initial estimate for space needed
    size_t var_name_len;
    char *env_value;
    
    result = malloc(alloc_size);
    if (!result)
        return NULL;
        
    while (i < len)
    {
        if (str[i] == '$' && i + 1 < len)
        {
            if (str[i + 1] == '{') {
                // Handle ${variable} syntax
                i += 2; // Skip ${
                size_t start = i;
                
                // Find the closing }
                while (i < len && str[i] != '}')
                    i++;
                
                if (i >= len || str[i] != '}') {
                    // No closing brace found, treat as literal
                    result[j++] = '$';
                    result[j++] = '{';
                    i = start;
                    continue;
                }
                
                var_name_len = i - start;
                if (var_name_len > 0) {
                    env_value = get_env_value(str + start, var_name_len);
                    
                    if (env_value)
                    {
                        size_t value_len = strlen(env_value);
                        // Ensure we have enough space
                        if (j + value_len >= alloc_size)
                        {
                            alloc_size = (j + value_len) * 2;
                            char *new_result = realloc(result, alloc_size);
                            if (!new_result)
                            {
                                free(result);
                                free(env_value);
                                return NULL;
                            }
                            result = new_result;
                        }
                        strcpy(result + j, env_value);
                        j += value_len;
                        free(env_value);
                    }
                }
                i++; // Skip the closing }
            }
            else if (isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?') {
                // Handle $variable syntax
                i++; // Skip the $
                var_name_len = get_var_name_len(str + i);
                env_value = get_env_value(str + i, var_name_len);
                
                if (env_value)
                {
                    size_t value_len = strlen(env_value);
                    // Ensure we have enough space
                    if (j + value_len >= alloc_size)
                    {
                        alloc_size = (j + value_len) * 2;
                        char *new_result = realloc(result, alloc_size);
                        if (!new_result)
                        {
                            free(result);
                            free(env_value);
                            return NULL;
                        }
                        result = new_result;
                    }
                    strcpy(result + j, env_value);
                    j += value_len;
                    free(env_value);
                }
                i += var_name_len;
            }
            else {
                // $ not followed by valid variable character, treat as literal
                if (j + 1 >= alloc_size)
                {
                    alloc_size *= 2;
                    char *new_result = realloc(result, alloc_size);
                    if (!new_result)
                    {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[j++] = str[i++];
            }
        }
        else
        {
            if (j + 1 >= alloc_size)
            {
                alloc_size *= 2;
                char *new_result = realloc(result, alloc_size);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
    
    // Shrink to actual size needed
    char *final = realloc(result, j + 1);
    return final ? final : result;
} 