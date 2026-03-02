/*
** EPITECH PROJECT, 2025
** B-YEP-Zappy
** File description:
** test_parsing.c
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../server/server.h" 

TestSuite(parsing);

Test(parsing, test_valid_arguments, .init = cr_redirect_stderr)
{
    char *argv[] = {
        "./zappy_server",
        "-p", "4242",
        "-x", "20",
        "-y", "20",
        "-n", "TeamA", "TeamB",
        "-c", "5",
        "-f", "100",
        NULL
    };
    int argc = 12;
    arguments_t args = {0};

    bool result = parse_args(argc, argv, &args);

    cr_assert(result == true, "parse_args should return true for valid arguments");
    cr_assert(args.port == 4242, "Port was not parsed correctly");
    cr_assert(args.width == 20, "Width was not parsed correctly");
    cr_assert(args.height == 20, "Height was not parsed correctly");
    cr_assert(args.clients_nb == 5, "Clients per team was not parsed correctly");
    cr_assert(args.freq == 100, "Frequency was not parsed correctly");
    cr_assert_str_eq(args.team_names[0], "TeamA", "First team name is incorrect");
    cr_assert_str_eq(args.team_names[1], "TeamB", "Second team name is incorrect");
    cr_assert(args.team_names[2] == NULL, "Team names array should be null-terminated");
}

Test(parsing, test_invalid_flag, .init = cr_redirect_stderr)
{
    char *argv[] = {"./zappy_server", "-z", "invalid", NULL};
    int argc = 3;
    arguments_t args = {0};

    bool result = parse_args(argc, argv, &args);

    cr_assert(result == false, "parse_args should return false for invalid flags");
}
