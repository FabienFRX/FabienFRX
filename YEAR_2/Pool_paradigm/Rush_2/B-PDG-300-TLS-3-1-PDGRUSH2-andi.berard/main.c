#include <stdio.h>
#include "char.h"
#include "new.h"

void test_char_operations()
{
    Object *char1 = new(Char, '0');
    Object *char2 = new(Char, '0');
    Object *result;

    // Afficher les objets initiaux
    printf("char1: %s\n", str(char1));
    printf("char2: %s\n", str(char2));

    // Tester l'addition
    printf("Testing addition:\n");
    result = addition(char1, char2);
    printf("%s + %s = %s\n", str(char1), str(char2), str(result));
    delete(result);

    // Tester la soustraction
    printf("Testing subtraction:\n");
    result = subtraction(char1, char2);
    printf("%s - %s = %s\n", str(char1), str(char2), str(result));
    delete(result);

    // Tester la multiplication
    printf("Testing multiplication:\n");
    result = multiplication(char1, char2);
    printf("%s * %s = %s\n", str(char1), str(char2), str(result));
    delete(result);

    // Tester la division
    printf("Testing division:\n");
    result = division(char1, char2);
    printf("%s / %s = %s\n", str(char1), str(char2), str(result));
    delete(result);

    // Tester l'égalité
    printf("Testing equality:\n");
    printf("%s == %s: %s\n", str(char1), str(char2),
           (eq(char1, char2) ? "true" : "false"));

    // Tester inférieur à
    printf("Testing less than:\n");
    printf("%s < %s: %s\n", str(char1), str(char2),
           (lt(char1, char2) ? "true" : "false"));

    // Tester supérieur à
    printf("Testing greater than:\n");
    printf("%s > %s: %s\n", str(char1), str(char2),
           (gt(char1, char2) ? "true" : "false"));

    // Libérer la mémoire
    delete(char1);
    delete(char2);
}

int main()
{
    test_char_operations();
    return 0;
}
