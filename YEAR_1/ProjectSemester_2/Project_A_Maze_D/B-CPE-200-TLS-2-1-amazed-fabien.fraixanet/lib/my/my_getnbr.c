/*
** EPITECH PROJECT, 2023
** MY_GETNBR
** File description:
** getnbr function
*/
static int check_is_num(const char *str, int index, int what_check)
{
    if (what_check == 1) {
        if (str[index] <= '9' && str[index] >= '1') {
            return 1;
        }
    }
    if (what_check == 0) {
        if (str[index] <= '9' && str[index] >= '0') {
            return 1;
        }
    }
    return 0;
}

static int check_is_char(const char *str, int index)
{
    if (str[index] < '0' || str[index] > '9') {
        return 1;
    }
    return 0;
}

static int check_is_operator(const char *str, int index)
{
    if (str[index] == '+' || str[index] == '-') {
        return 1;
    }
    return 0;
}

static long long int apply_sign_number(long long int number, int nb_subs)
{
    if ((nb_subs % 2) != 0) {
        number = -number;
    }
    return number;
}

static void check_max_or_min(long long int *number)
{
    if (*number < -2147483648 || *number > 2147483647) {
        *number = 0;
    }
}

static long long int take_number(const char *str, int index)
{
    long long int number = 0;
    int convert_int = 0;

    while (check_is_num(str, index, 0) == 1) {
        convert_int = str[index] - '0';
        number += convert_int;
        number *= 10;
        index++;
    }
    number /= 10;
    return number;
}

static long long int course_array(const char *str,
    long long int number, int index, int *nb_subs)
{
    while (check_is_char(str, index) == 0 ||
        check_is_operator(str, index) == 1) {
        if (str[index] == '0' && check_is_char(str, (index +1)) == 1) {
            return 0;
        }
        if (check_is_num(str, index, 1) == 1) {
            number = take_number(str, index);
            return number;
        }
        if (str[index] == '-') {
            *nb_subs += 1;
        }
        index++;
    }
    return number;
}

static void get_nbr(const char *str, long long int *number)
{
    int index = 0;
    int nb_subs = 0;

    *number = course_array(str, *number, index, &nb_subs);
    *number = apply_sign_number(*number, nb_subs);
    check_max_or_min(number);
}

int my_getnbr(const char *str)
{
    long long int number = 0;

    if (str[0] == '\0' || str == 0) {
        return 0;
    }
    get_nbr(str, &number);
    return (int)number;
}
