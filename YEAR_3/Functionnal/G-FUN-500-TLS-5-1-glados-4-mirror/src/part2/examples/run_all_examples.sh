#!/bin/bash

cd ../Build

echo "========================================="
echo "  Running All GLaDOS Examples"
echo "========================================="
echo ""

passed=0
failed=0

for file in ../examples/*.gld; do
    basename=$(basename "$file")
    echo -n "Testing $basename ... "

    if ./glados-compiler -o test.bc "$file" 2>/dev/null; then
        result=$(./glados-vm test.bc 2>/dev/null)
        echo "PASS (output: $result)"
        ((passed++))
    else
        echo "FAIL"
        ((failed++))
    fi
done

echo ""
echo "========================================="
echo "  Results"
echo "========================================="
echo "Passed: $passed"
echo "Failed: $failed"
echo "Total:  $((passed + failed))"
echo "========================================="

# Clean up
rm -f test.bc
