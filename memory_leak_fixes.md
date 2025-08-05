# Memory Leak Analysis and Fixes for Minishell

## Summary of Issues

1. **Environment array leak**: 778 bytes in 3 blocks (definitely lost)
2. **Invalid memory access**: Use-after-free when environment is modified
3. **Double-free errors**: In PWD update functions

## Detailed Analysis

### 1. Environment Array Management Issue

**Where it happens**: In `add_new_var()` function (utils3_helper5.c)

When a new environment variable is added:
```c
// Current flow:
1. Allocate new array: new_environ = malloc((count + 2) * sizeof(char *))
2. Copy pointers from old array to new array
3. Free old array: free(*env_copy)
4. Set *env_copy = new_environ
```

**The problem**: This is correct for a single operation, but when multiple exports happen in sequence, there's no issue with the array itself. The leak must be from something else.

### 2. The Real Leak Source

Looking at the valgrind output more carefully, the 778-byte leak comes from:
- `allocate_new_environ` called from `add_new_var`
- This happens when exporting variables

The issue is likely that when a builtin command modifies the environment and then the command structure is freed, the new environment array might be leaked if the builtin exits early or if there's an error path that doesn't properly update the main environment pointer.

### 3. Use-After-Free Issues

The invalid reads happen in `search_env_variable` after the environment has been modified. This suggests that:
- The environment is being accessed after it has been freed and replaced
- This happens because the environment pointer is not properly synchronized between different parts of the code

## Fixes Required

### Fix 1: Ensure Environment Updates Are Atomic

The environment should only be updated at well-defined points, and all references should be updated atomically.

### Fix 2: Add Proper Error Handling

In `add_new_var` and similar functions, ensure that on error paths, allocated memory is properly freed.

### Fix 3: Fix the PWD Update Logic

The `update_pwd_variables` function has a double-free issue. The problem is that it's trying to free memory that was already freed when the environment was updated.

### Fix 4: Centralize Environment Management

Instead of passing `char ***env_copy` around and allowing any function to modify it, consider:
1. Having a single global environment manager
2. Or ensuring that environment updates only happen in the main loop

## Specific Code Fixes

### In `builtin_cd_helper.c`:

The `update_pwd_and_cleanup` function should not free `old_pwd` if it's already been incorporated into the environment.

### In `utils3_helper2.c`:

The `finalize_new_environ` function should ensure that the old environment array is only freed once and that all pointers are properly updated.

### In main loop:

Ensure that after each command execution, the environment pointer is consistent and that any temporary allocations are freed.

## Testing

To verify the fixes:
1. Run: `export TEST=1; export TEST=2; export TEST=3`
2. Run: `cd /tmp; cd /; cd /tmp`
3. Run: `export A=1; unset A; export A=2`

Each of these should complete without memory leaks or invalid memory access.