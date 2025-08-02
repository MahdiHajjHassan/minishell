#!/bin/bash

echo "Testing minishell quote handling issues:"
echo ""

echo "Test 1: echo hello (should work)"
echo 'echo hello' | ./minishell

echo ""
echo "Test 2: \"echo\" hello (quoted command - should work but doesn't)"
echo '"echo" hello' | ./minishell

echo ""
echo "Test 3: echo \"hello\" (quoted argument - should print hello but prints nothing)"
echo 'echo "hello"' | ./minishell

echo ""
echo "Test 4: echo \"\" (empty quotes - should print empty line)"
echo 'echo ""' | ./minishell

echo ""
echo "For comparison, bash output:"
echo "1. echo hello:"
echo hello
echo "2. \"echo\" hello:"
"echo" hello
echo "3. echo \"hello\":"
echo "hello"
echo "4. echo \"\"":
echo ""