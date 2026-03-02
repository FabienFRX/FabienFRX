/*
** EPITECH PROJECT, 2023
** 103cipher
** File description:
** 103cipher for encrypt and decrypt argument
*/


#include "include/my.h"

char *strdup(const char *c)
{
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

param_t *set_struct(char *argv[])
{
    param_t *params = malloc(sizeof(*params));
    
    params->msg = strdup(argv[1]);
    params->key = strdup(argv[2]);
    params->key_len = strlen(argv[2]);
    params->key_matrix = NULL;
    if (atoi(argv[3]) == 0) {
        params->type = encrypt;
    } else {
        params->type = decrypt;
    }
    return params;
}

void matrix_key(param_t *params)
{
    int offset = 0;
    int i;

    if (params->key_len > 1 && params->key_len <= 4)
        offset = 4;
    else if (params->key_len > 4 && params->key_len <= 9)
        offset = 9;
    else if (params->key_len > 9 && params->key_len <= 16)
        offset = 16;
    params->key_matrix = malloc(sizeof(double) * offset);
    if (offset == 16)
        offset = 12;
    for (i = 0; i < params->key_len; i++)
        params->key_matrix[i] = params->key[i];
    for (; params->key_len < offset; params->key_len++)
        params->key_matrix[params->key_len] = 0;
}

void cipher(param_t *params)
{
    if (params->type == encrypt) {
		if (params->key_len > 1) {
			matrix_key(params);
			encryption(params);
		} else {
			one_size_encryption(params);
		}
	} else if (params->type == decrypt) {
		if (params->key_len > 1) {
			matrix_key(params);
			//decryption(params);
		} else {
			one_size_decryption(params);
		}
	} else {
		exit(84);
	}
}

int main(int argc, char **argv)
{
    param_t *params;

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        display_h();
        return (0);
    } else if (argc != 4) {
        return (84);
    } else {
        int num = atoi(argv[3]);
        if (!argv[1][0] || !argv[2][0] || strlen(argv[3]) != 1 || (num != 0 && num != 1)) {
			return 84;
		}
    }
    params = set_struct(argv);
    cipher(params);
    free(params->key_matrix);
    free(params->msg);
    free(params->key);
    free(params);
    return 0;
}
