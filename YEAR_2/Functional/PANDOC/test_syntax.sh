#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to test conversion
test_conversion() {
    local input=$1
    local output_format=$2
    local expected_exit_code=${3:-0}
    
    echo -n "Testing ${input##*/} to $output_format... "
    
    ./mypandoc -i "$input" -f "$output_format" -o "test_output.$output_format" 2>/dev/null
    exit_code=$?
    
    if [ $exit_code -eq $expected_exit_code ]; then
        echo -e "${GREEN}PASSED${NC}"
        return 0
    else
        echo -e "${RED}FAILED${NC} (exit code: $exit_code, expected: $expected_exit_code)"
        return 1
    fi
}

# Run tests
echo "Starting MyPandoc tests..."
echo

# Test valid conversions
test_conversion "examples/syntaxe.json" "xml"
test_conversion "examples/syntaxe.json" "markdown"
test_conversion "examples/syntaxe.xml" "json"
test_conversion "examples/syntaxe.xml" "markdown"
test_conversion "examples/syntaxe.md" "json"
test_conversion "examples/syntaxe.md" "xml"

# Test error cases
echo
echo "Testing error cases..."
./mypandoc 2>/dev/null
if [ $? -eq 84 ]; then
    echo -e "No arguments: ${GREEN}PASSED${NC}"
else
    echo -e "No arguments: ${RED}FAILED${NC}"
fi

./mypandoc -i examples/nonexistent.json -f xml 2>/dev/null
if [ $? -eq 84 ]; then
    echo -e "Nonexistent file: ${GREEN}PASSED${NC}"
else
    echo -e "Nonexistent file: ${RED}FAILED${NC}"
fi

# Clean up
rm -f test_output.* temp.xml roundtrip.json

echo
echo "Tests completed!"