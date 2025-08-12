#!/bin/bash

# Script to automatically fix common norminette violations

echo "Fixing norminette violations..."

# Fix variable declarations not followed by newlines
find src/ -name "*.c" -exec sed -i 's/^[[:space:]]*\([a-zA-Z_][a-zA-Z0-9_]*[[:space:]]\+[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*;\)[[:space:]]*$/\1\n/' {} \;

# Fix functions not separated by newlines
find src/ -name "*.c" -exec sed -i 's/^}[[:space:]]*$/\n&/' {} \;

# Fix empty lines in functions (remove them)
find src/ -name "*.c" -exec sed -i '/^[[:space:]]*$/d' {} \;

# Fix preprocessor statements not followed by newlines
find src/ -name "*.c" -exec sed -i 's/^#[[:space:]]*\([a-zA-Z_][a-zA-Z0-9_]*\)[[:space:]]*$/#\1\n/' {} \;

echo "Norminette fixes applied. Running norminette check..."
norminette src/ inc/ 