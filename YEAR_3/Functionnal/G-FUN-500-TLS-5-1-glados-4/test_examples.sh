#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXAMPLES_DIR="$SCRIPT_DIR/examples"
GLADOS_BINARY="$SCRIPT_DIR/glados"

echo -e "${BLUE}=== GLaDOS Scheme Interpreter Test Suite ===${NC}"
echo

# Check if glados binary exists, if not try to build it
if [ ! -f "$GLADOS_BINARY" ]; then
    echo -e "${YELLOW}GLaDOS binary not found. Attempting to build...${NC}"
    
    # Try building with stack first
    if command -v stack &> /dev/null; then
        echo -e "${BLUE}Building with stack...${NC}"
        if stack build; then
            # Find the built binary
            STACK_BINARY=$(stack exec which glados 2>/dev/null)
            if [ -n "$STACK_BINARY" ]; then
                cp "$STACK_BINARY" "$GLADOS_BINARY"
                echo -e "${GREEN}✓ Built successfully with stack${NC}"
            else
                echo -e "${RED}✗ Failed to locate stack-built binary${NC}"
                exit 1
            fi
        else
            echo -e "${RED}✗ Stack build failed${NC}"
            exit 1
        fi
    # Try building with cabal
    elif command -v cabal &> /dev/null; then
        echo -e "${BLUE}Building with cabal...${NC}"
        if cabal build; then
            # Find the built binary
            CABAL_BINARY=$(find dist-newstyle -name "glados" -type f -executable 2>/dev/null | head -1)
            if [ -n "$CABAL_BINARY" ]; then
                cp "$CABAL_BINARY" "$GLADOS_BINARY"
                echo -e "${GREEN}✓ Built successfully with cabal${NC}"
            else
                echo -e "${RED}✗ Failed to locate cabal-built binary${NC}"
                exit 1
            fi
        else
            echo -e "${RED}✗ Cabal build failed${NC}"
            exit 1
        fi
    else
        echo -e "${RED}✗ Neither stack nor cabal found. Please install one of them.${NC}"
        exit 1
    fi
    echo
fi

# Make sure the binary is executable
chmod +x "$GLADOS_BINARY"

# Check if examples directory exists
if [ ! -d "$EXAMPLES_DIR" ]; then
    echo -e "${RED}✗ Examples directory not found: $EXAMPLES_DIR${NC}"
    exit 1
fi

# Find all .scm files in examples directory
SCM_FILES=($(find "$EXAMPLES_DIR" -name "*.scm" -type f | sort))

if [ ${#SCM_FILES[@]} -eq 0 ]; then
    echo -e "${YELLOW}No .scm files found in $EXAMPLES_DIR${NC}"
    exit 1
fi

echo -e "${BLUE}Found ${#SCM_FILES[@]} .scm files to test:${NC}"
for file in "${SCM_FILES[@]}"; do
    echo -e "  - $(basename "$file")"
done
echo

# Test results counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test each .scm file
for scm_file in "${SCM_FILES[@]}"; do
    filename=$(basename "$scm_file")
    echo -e "${BLUE}Testing: $filename${NC}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run glados on the file and capture output
    if output=$("$GLADOS_BINARY" "$scm_file" 2>&1); then
        echo -e "${GREEN}✓ PASS${NC} - $filename"
        if [ -n "$output" ]; then
            echo -e "${GREEN}Output:${NC}"
            echo "$output" | sed 's/^/  /'
        fi
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        exit_code=$?
        echo -e "${RED}✗ FAIL${NC} - $filename (exit code: $exit_code)"
        if [ -n "$output" ]; then
            echo -e "${RED}Error output:${NC}"
            echo "$output" | sed 's/^/  /'
        fi
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo
done

# Print summary
echo -e "${BLUE}=== Test Summary ===${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
if [ $FAILED_TESTS -gt 0 ]; then
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
else
    echo -e "${GREEN}Failed: $FAILED_TESTS${NC}"
fi
echo

# Exit with appropriate code
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed.${NC}"
    exit 1
fi