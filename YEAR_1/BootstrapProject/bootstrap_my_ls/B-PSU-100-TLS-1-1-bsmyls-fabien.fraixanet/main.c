/*
** EPITECH PROJECT, 2023
** main
** File description:
** main
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct {
    char name [100];
    char size;
} file;

void affichage(file file)
{
    printf("Name : %s\n", file.name);
    printf("Size : %d\n", file.size);
}

int main(void)
{
    file file1;
    file1.name;
    file1.size;
    strcpy(file1.name, "info_file");

    affichage(file1);
    
/*    struct stat file;
      struct((unsigned char *)&file);
      stat("./info_file", &file);
      struct((unsigned char *)&file);

      printf("File Size:\t%lu bytes\n", file.st_size);
      info_files(".");*/
    return(0);
}
