/*
** EPITECH PROJECT, 2023
** encryption
** File description:
** azerty
*/
#include "include/my.h"

void encrypt_2_for_2(param_t *params)
{
	int msg_len = strlen(params->msg);
	int i = 0, j = 0, fir = 0, sec = 0;

	for (i = 0, j = 1; i < msg_len; i += 2, j += 2) {
		fir = params->msg[i] * params->key_matrix[0];
		fir += params->msg[j] * params->key_matrix[2];
		sec = params->msg[i] * params->key_matrix[1];
		sec += params->msg[j] * params->key_matrix[3];
		printf("%d %d", fir, sec);
		if (j < msg_len - 2)
			printf(" ");
	}
	if (i < msg_len) {
		fir = params->msg[i] * params->key_matrix[0];
		sec = params->msg[j] * params->key_matrix[1];

		printf(" %d %d", fir, sec);
	}
	printf("\n");
}

void encrypt_3_for_3(param_t *params)
{
	int msg_len = strlen(params->msg);
	int i = 0, j = 0, k = 0, fir = 0, sec = 0, thr = 0;

	for (i = 0, j = 1, k = 2; i < msg_len; i += 3, j += 3, k += 3) {
		fir = params->msg[i] * params->key_matrix[0];
		fir += params->msg[j] * params->key_matrix[3];
		fir += params->msg[k] * params->key_matrix[6];
		sec = params->msg[i] * params->key_matrix[1];
		sec += params->msg[j] * params->key_matrix[4];
		sec += params->msg[k] * params->key_matrix[7];
		thr = params->msg[i] * params->key_matrix[2];
		thr += params->msg[j] * params->key_matrix[5];
		thr += params->msg[k] * params->key_matrix[8];
		printf("%d %d %d", fir, sec, thr);
		if (k < msg_len - 2)
			printf(" ");
	}
	if (i < msg_len || (j > i && j < msg_len)) {
		fir = params->msg[i] * params->key_matrix[0];
		sec = params->msg[i] * params->key_matrix[1];
		thr = params->msg[i] * params->key_matrix[2];
		printf(" %d %d %d", fir, sec, thr);
	}
	printf("\n");
}

void encryption(param_t *params)
{
	int i;

	if (params->key_len > 16) {
		exit(84);
	}
	double *key = params->key_matrix;
	printf("Key matrix:\n");
	if (params->key_len <= 4) {
		for (i = 0; i < 4; i += 2)
			printf("%.0f\t%.0f\n", key[i], key[i + 1]);
		printf("\nEncrypted message:\n");
		encrypt_2_for_2(params);
	} else if (params->key_len > 4 && params->key_len <= 9) {
		for (i = 0; i < 9; i += 3)
			printf("%.0f\t%.0f\t%.0f\n", key[i], key[i + 1], key[i + 2]);

		printf("\nEncrypted message:\n");
		encrypt_3_for_3(params);
	} else if (params->key_len > 9 && params->key_len <= 16) {
		for (i = 0; i < 16; i += 4)
			printf("%.0f\t%.0f\t%.0f\t%.0f\n", key[i], key[i + 1], key[i + 2], key[i + 3]);
		
		printf("\nEncrypted message:\n");
		printf("key = %s\n", params->key);
	}
}
