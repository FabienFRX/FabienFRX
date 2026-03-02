#!/bin/bash

# Test 1: Sorted input, no operations
echo "Test 1: Sorted input, no operations"
echo "" | ./pushswap_checker 1 2 3 4 5
echo "Expected: OK"
echo ""

# Test 2: Unsorted input, operations to sort
echo "Test 2: Unsorted input, operations to sort"
echo "pb pb sa pa pa" | ./pushswap_checker 2 1 3
echo "Expected: KO"
echo ""

# Test 3: Unsorted input, no operations
echo "Test 3: Unsorted input, no operations"
echo "" | ./pushswap_checker 3 2 1
echo "Expected: KO"
echo ""

# Test 4: Invalid operation
echo "Test 4: Invalid operation"
echo "invalid_op" | ./pushswap_checker 1 2 3
echo "Expected: KO: Invalid operation"
echo ""

# Test 5: No input numbers provided
echo "Test 5: No input numbers provided"
echo "" | ./pushswap_checker
echo "Expected: KO: No input numbers provided"
echo ""

# Test 6: No operations provided
echo "Test 6: No operations provided"
echo "" | ./pushswap_checker 1 2 3
echo "Expected: OK"
echo ""

# Test 7: Invalid number format
echo "Test 7: Invalid number format"
echo "" | ./pushswap_checker 1 2 a
echo "Expected: KO: Invalid number format"
echo ""
