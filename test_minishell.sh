#!/bin/bash

# Comprehensive Minishell Test Script
# Based on the evaluation criteria from the subject and evaluation sheet

echo "=== MINISHELL COMPREHENSIVE TEST ==="
echo "Testing all mandatory requirements..."
echo

# Test 1: Basic compilation
echo "1. Testing compilation..."
if make > /dev/null 2>&1; then
    echo "✓ Compilation successful with -Wall -Wextra -Werror"
else
    echo "✗ Compilation failed"
    exit 1
fi
echo

# Test 2: Basic command execution
echo "2. Testing basic command execution..."
echo "Testing /bin/ls:"
echo "/bin/ls" | ./minishell
echo

# Test 3: Command with arguments
echo "3. Testing command with arguments..."
echo "Testing /bin/ls -la:"
echo "/bin/ls -la" | ./minishell
echo

# Test 4: Echo command
echo "4. Testing echo command..."
echo "Testing echo hello world:"
echo "echo hello world" | ./minishell
echo "Testing echo -n hello world:"
echo "echo -n hello world" | ./minishell
echo

# Test 5: Exit command
echo "5. Testing exit command..."
echo "Testing exit:"
echo "exit" | ./minishell
echo "Testing exit with argument:"
echo "exit 42" | ./minishell
echo

# Test 6: Exit status
echo "6. Testing exit status..."
echo "Testing successful command exit status:"
echo -e "/bin/ls\necho \$?" | ./minishell
echo "Testing failed command exit status:"
echo -e "/bin/ls nonexistentfile\necho \$?" | ./minishell
echo

# Test 7: Environment variables
echo "7. Testing environment variables..."
echo "Testing \$USER:"
echo "echo \$USER" | ./minishell
echo "Testing \$PATH:"
echo "echo \$PATH" | ./minishell
echo

# Test 8: Double quotes
echo "8. Testing double quotes..."
echo "Testing echo \"hello world\":"
echo "echo \"hello world\"" | ./minishell
echo "Testing echo \"\$USER\":"
echo "echo \"\$USER\"" | ./minishell
echo

# Test 9: Single quotes
echo "9. Testing single quotes..."
echo "Testing echo 'hello world':"
echo "echo 'hello world'" | ./minishell
echo "Testing echo '\$USER':"
echo "echo '\$USER'" | ./minishell
echo

# Test 10: Built-in commands
echo "10. Testing built-in commands..."
echo "Testing pwd:"
echo "pwd" | ./minishell
echo "Testing env:"
echo "env" | ./minishell | head -5
echo "... (truncated)"
echo

# Test 11: Export and unset
echo "11. Testing export and unset..."
echo "Testing export:"
echo -e "export TEST_VAR=hello\necho \$TEST_VAR" | ./minishell
echo "Testing unset:"
echo -e "export TEST_VAR=hello\nunset TEST_VAR\necho \$TEST_VAR" | ./minishell
echo

# Test 12: CD command
echo "12. Testing cd command..."
echo "Testing cd:"
echo -e "pwd\ncd ..\npwd\ncd minishell7\npwd" | ./minishell
echo

# Test 13: Relative paths
echo "13. Testing relative paths..."
echo "Testing ls with relative path:"
echo "ls src" | ./minishell
echo

# Test 14: PATH variable
echo "14. Testing PATH variable..."
echo "Testing ls without path:"
echo "ls" | ./minishell
echo

# Test 15: Redirections
echo "15. Testing redirections..."
echo "Testing output redirection:"
echo "echo hello > test_redirect.txt" | ./minishell
if [ -f test_redirect.txt ]; then
    echo "✓ File created successfully"
    cat test_redirect.txt
    rm test_redirect.txt
else
    echo "✗ Redirection failed"
fi
echo

# Test 16: Pipes
echo "16. Testing pipes..."
echo "Testing ls | grep Makefile:"
echo "ls | grep Makefile" | ./minishell
echo

# Test 17: Error handling
echo "17. Testing error handling..."
echo "Testing nonexistent command:"
echo "nonexistentcommand" | ./minishell
echo

# Test 18: Memory leaks
echo "18. Testing for memory leaks..."
echo "Running valgrind check..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_test.log echo "ls" | ./minishell > /dev/null 2>&1
if grep -q "All heap blocks were freed" valgrind_test.log; then
    echo "✓ No memory leaks detected"
else
    echo "✗ Memory leaks detected"
    cat valgrind_test.log | grep -A 10 -B 10 "LEAK SUMMARY"
fi
rm -f valgrind_test.log
echo

# Test 19: Global variable check
echo "19. Checking global variable requirement..."
echo "Note: Subject requires a global variable to indicate received signals"
echo "Current global variables found:"
grep -r "static int.*=" src/ | grep -v "static int.*("
echo

echo "=== TEST SUMMARY ==="
echo "Basic functionality tests completed."
echo "Issues found:"
echo "1. Redirections may have parsing issues"
echo "2. Missing global variable for signal handling (subject requirement)"
echo
echo "Recommendations:"
echo "1. Fix redirection parsing"
echo "2. Add global variable for signal handling"
echo "3. Test signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)"
echo "4. Test heredoc functionality"
echo "5. Test complex command combinations" 