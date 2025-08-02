#!/bin/bash

# Heredoc Test Script for Minishell
# Tests << operator functionality

echo "=== HEREDOC TESTS ==="
echo

echo "1. Testing basic heredoc:"
echo "cat << EOF"
echo "line1"
echo "line2"
echo "EOF"
echo "Expected: Should read input until EOF delimiter"
echo "cat << EOF" | ./minishell
echo

echo "2. Testing heredoc with variable expansion:"
echo "cat << EOF"
echo "User: \$USER"
echo "Path: \$PATH"
echo "EOF"
echo "Expected: Should expand variables in heredoc"
echo "cat << EOF" | ./minishell
echo

echo "3. Testing heredoc with quoted delimiter:"
echo "cat << 'EOF'"
echo "User: \$USER"
echo "Path: \$PATH"
echo "EOF"
echo "Expected: Should NOT expand variables with quoted delimiter"
echo "cat << 'EOF'" | ./minishell
echo

echo "4. Testing heredoc in pipeline:"
echo "cat << EOF | grep line"
echo "line1"
echo "line2"
echo "test"
echo "EOF"
echo "Expected: Should work with pipes"
echo "cat << EOF | grep line" | ./minishell
echo

echo "5. Testing heredoc with redirection:"
echo "cat << EOF > heredoc_output.txt"
echo "test content"
echo "EOF"
echo "Expected: Should create file with heredoc content"
echo "cat << EOF > heredoc_output.txt" | ./minishell
if [ -f heredoc_output.txt ]; then
    echo "✓ File created successfully"
    cat heredoc_output.txt
    rm heredoc_output.txt
else
    echo "✗ File creation failed"
fi
echo

echo "=== HEREDOC TEST SUMMARY ==="
echo "Note: Heredoc functionality may not be fully implemented"
echo "Check if << operator is recognized and processed correctly" 