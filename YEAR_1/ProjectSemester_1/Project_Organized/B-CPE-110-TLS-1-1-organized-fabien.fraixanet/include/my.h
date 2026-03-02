/*
** EPITECH PROJECT, 2023
** my_h
** File description:
** header that contains all prototypes
*/

#include <sys/types.h>
#ifndef MY_H
    #define MY_H

// project
typedef enum type {
    ACTUATOR,
    DEVICE,
    PROCESSOR,
    SENSOR,
    WIRE
} type_t;

static const int type_value_lenght = 5;

static const char *type_value[] = {
    "ACTUATOR",
    "DEVICE",
    "PROCESSOR",
    "SENSOR",
    "WIRE",
};

typedef struct object_s {
    int id;
    char *name;
    type_t type;
} object_t;

typedef enum object_field {
    ID,
    NAME,
    TYPE,
} object_field_t;

static const int object_field_value_length = 3;

static const char *object_field_value[] = {
    "ID",
    "NAME",
    "TYPE",
};

typedef struct linked_list_s {
    object_t *data;
    struct linked_list_s *next;
} linked_list_t;

// lib
void my_putchar(char c);
char *my_strstr(char *str, char const *to_find);
int my_isneg(int nb);
int my_strcmp(char const *s1, char const *s2);
int my_put_nbr(int nb);
int my_put_nbr_hexa(int nb);
int my_put_nbr_u(int nb);
int my_put_nbr_base(int nb, int base);
int my_put_nbr_base_cap(int nb, int base);
int my_put_nbr_base_low(int nb, int base);
int my_strncmp(char const *s1, char const *s2, int n);
void my_swap(int *a, int *b);
char *my_strupcase(char *str);
int my_putstr(char const *str);
char *my_strlowcase(char *str);
int my_strlen(char const *str);
char *my_strcapitalize(char *str);
int my_getnbr(char const *str);
int my_str_isalpha(char const *str);
void my_sort_int_array(int *tab, int size);
int my_str_isnum(char const *str);
int my_compute_power_rec(int nb, int power);
int my_str_islower(char const *str);
int my_compute_square_root(int nb);
int my_str_isupper(char const *str);
int my_is_prime(int nb);
int my_str_isprintable(char const *str);
int my_find_prime_sup(int nb);
int my_showstr(char const *str);
char *my_strcpy(char *dest, char const *src);
int my_showmem(char const *str, int size);
char *my_strncpy(char *dest, char const *src, int n);
char *my_strcat(char *dest, char const *src);
char *my_revstr(char *str);
char *my_strncat(char *dest, char const *src, int nb);
int min(int n1, int n2);
int max(int n1, int n2);
char *my_strdup(char const *src);
void print_p(void *ptr);
int my_printf(const char *format, ...);
void my_print_float(double f, int decimals);
int my_put_nbr_display(int nb);
void my_print_e_low(double f);
float my_print_e(int *exponent, float tmp);
void my_print_e_cap(double f);
char *get_path(char **argv);
int different_option(char option, char *path);
int different_option_2(char option, char *path);
int my_ls(char *path);
int option_a(char *path);
int option_d(char *path);
int permission(mode_t protect);
int option_r(char *path);
#endif /*MY_H*/
