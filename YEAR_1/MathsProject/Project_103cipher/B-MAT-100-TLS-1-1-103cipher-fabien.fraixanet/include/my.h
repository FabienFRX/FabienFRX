/*
** EPITECH PROJECT, 2023
** my.h
** File description:
** azerty
*/

#ifndef MY_H_
    #define MY_H_
    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <ctype.h>
    #include <stdbool.h>
    #define M_PI 3.14159265358979323846

typedef enum type {
    encrypt,
    decrypt
}type_t;

typedef struct params {
	double 	*key_matrix;
	int 	key_len;
	type_t 	type;
	char 	*msg;
	char 	*key;
} param_t;

void display_h(void);
void encryption(param_t *params);
void encrypt_3_for_3(param_t *params);
void encrypt_2_for_2(param_t *params);
void decryption(param_t *params);
void one_size_encryption(param_t *params);
void one_size_decryption(param_t *param);

#endif /* !MY_H_ */
