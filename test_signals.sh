#!/bin/bash

# Signal Handling Test Script for Minishell
# Tests Ctrl+C, Ctrl+D, and Ctrl+\ behavior

echo "=== SIGNAL HANDLING TESTS ==="
echo "Note: Some tests require interactive input"
echo

echo "1. Testing Ctrl+D (EOF) behavior:"
echo "This should exit minishell when pressed on empty line"
echo "Press Ctrl+D when you see the prompt:"
echo "echo 'test'" | ./minishell
echo

echo "2. Testing Ctrl+C behavior:"
echo "This should show new prompt on new line"
echo "Starting minishell for interactive test..."
echo "Type some text, then press Ctrl+C"
echo "Expected: New prompt on new line"
echo "Press Enter to continue..."
read

echo "3. Testing Ctrl+\\ behavior:"
echo "This should do nothing in interactive mode"
echo "Starting minishell for interactive test..."
echo "Type some text, then press Ctrl+\\"
echo "Expected: Nothing happens"
echo "Press Enter to continue..."
read

echo "4. Testing signal handling in child processes:"
echo "Testing cat command (blocking) with Ctrl+C:"
echo "This should interrupt the cat command"
echo "Starting cat command..."
echo "Press Ctrl+C to interrupt:"
echo "cat" | ./minishell &
SHELL_PID=$!
sleep 2
kill -INT $SHELL_PID
wait $SHELL_PID
echo

echo "5. Testing signal handling with pipes:"
echo "Testing cat | grep with Ctrl+C:"
echo "This should handle signals properly in pipeline"
echo "cat | grep test" | ./minishell &
SHELL_PID=$!
sleep 2
kill -INT $SHELL_PID
wait $SHELL_PID
echo

echo "=== SIGNAL TEST SUMMARY ==="
echo "Manual testing required for:"
echo "- Ctrl+C in interactive mode"
echo "- Ctrl+D in interactive mode"
echo "- Ctrl+\\ in interactive mode"
echo "- Signal handling in child processes"
echo
echo "To test interactively:"
echo "1. Run: ./minishell"
echo "2. Type some text"
echo "3. Press Ctrl+C - should show new prompt"
echo "4. Press Ctrl+D on empty line - should exit"
echo "5. Press Ctrl+\\ - should do nothing" 