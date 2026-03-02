/*
** EPITECH PROJECT, 2025
** MinilibC
** File description:
** main
*/

#include <stdio.h>
#include <string.h>

int main() {
    printf("Test 1: %d\n", strncmp("hello", "hello", 5));       // 0
    printf("Test 2: %d\n", strncmp("hello", "hellp", 5));       // -1
    printf("Test 3: %d\n", strncmp("abc", "abd", 3));           // -1
    printf("Test 4: %d\n", strncmp("abd", "abc", 3));           // 1
    printf("Test 5: %d\n", strncmp("hello", "hello world", 8)); // 0
    printf("Test 6: %d\n", strncmp("bonjour", "bonsoir", 7));   // 1
    printf("Test 7: %d\n", strncmp("hello", "hell", 5));        // 1 (car "o" > "\0")
    printf("Test 8: %d\n", strncmp("abc", "abcde", 3));         // 0 (car n=3)
    printf("Test 9: %d\n", strncmp("test", "test", 0));         // 0 (car n=0)
    return 0;
}
