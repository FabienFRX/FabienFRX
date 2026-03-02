/*
** EPITECH PROJECT, 2023
** One seized decryption/encryption
** File description:
** azerty
*/
#include "include/my.h"

void one_size_encryption(param_t *params)
{
    int msg_len = strlen(params->msg);
    double key = params->key[0];

    printf("Key matrix :\n%.0f\n\n", key);
    printf("Encrypted message :\n");

    for (int i = 0; i < msg_len; i++) {
        double tmp = params->msg[i] * key;
        printf("%.0f", tmp);
        if (i < msg_len - 1)
            printf(" ");
    }
    printf("\n");
}

void one_size_decryption(param_t *param)
{
    int msg_len = strlen(param->msg);
    int current_number = 0;
    double key = param->key[0];
    int i = 0;

    printf("Key matrix :\n%.0f\n\n", key);
    printf("Decrypted message :\n");
    while (i < msg_len) {
        while (param->msg[i] != ' ' && param->msg[i] != '\0') {
            current_number = current_number * 10 + (param->msg[i] - '0');
            i++;
        }
        char decrypted_char = current_number / key;
        printf("%c", decrypted_char);
        i++;
        current_number = 0;
    }
    printf("\n");
}
