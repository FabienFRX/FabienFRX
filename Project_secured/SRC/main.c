/*
** EPITECH PROJECT, 2024
** B-CPE-110-TLS-1-1-secured-fabien.fraixanet
** File description:
** create_book
*/

#include "../include/hashtable.h"
#include "../include/my.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    hashtable_t *ht = new_hashtable(&hash, 4);

    if (!ht)
        return 84;
    ht_insert(ht, "Visions", "./Documents/Tournament/Modules/Vision");
    ht_insert(ht, "Kratos", "./Trash/Hollidays_Pics/.secret_folder/kratos.ai");
    ht_insert(ht, "<3", "+33 6 31 45 61 23 71");
    ht_delete(ht, "Visions");
    ht_dump(ht);
    my_printf("Looking for \"Kratos\": \"%s\"\n", ht_search(ht, "Kratos"));
    delete_hashtable(ht);
    return 0;
}
