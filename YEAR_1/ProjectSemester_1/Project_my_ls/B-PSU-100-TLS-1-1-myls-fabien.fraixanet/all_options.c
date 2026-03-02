/*
** EPITECH PROJECT, 2023
** option_l
** File description:
** option that take lot of info
*/

#include "include/my_ls.h"
#include "include/my.h"

void permission(mode_t mode)
{
    my_printf((S_ISDIR(mode)) ? "d" : "-");
    my_printf((mode & S_IRUSR) ? "r" : "-");
    my_printf((mode & S_IWUSR) ? "w" : "-");
    my_printf((mode & S_IXUSR) ? "x" : "-");
    my_printf((mode & S_IRGRP) ? "r" : "-");
    my_printf((mode & S_IWGRP) ? "w" : "-");
    my_printf((mode & S_IXGRP) ? "x" : "-");
    my_printf((mode & S_IROTH) ? "r" : "-");
    my_printf((mode & S_IWOTH) ? "w" : "-");
    my_printf((mode & S_IXOTH) ? "x" : "-");
}

void print_info(const char *all_path, const struct stat *all_info)
{
    struct passwd *pwd;
    struct group *grp;
    char *str_time;
    const char *username;
    const char *gropname;

    permission(all_info->st_mode);
    my_printf(" %d ", (long)all_info->st_nlink);
    pwd = getpwuid(all_info->st_uid);
    grp = getgrgid(all_info->st_gid);
    username = (pwd != NULL) ? pwd->pw_name : "";
    gropname = (grp != NULL) ? grp->gr_name : "";
    my_printf(" %s %s", username, gropname);
    my_printf(" %d", all_info->st_size);
    str_time = ctime(&all_info->st_mtime);
    str_time[my_strlen(str_time) -1] = '\0';
    my_printf(" %s", str_time);
    my_printf("%s\n", all_path);
}

void file_process(const char *path, const struct dirent *entity, int long_form)
{
    char all_path[PATH];
    struct stat all_info;

    my_strcpy(all_path, path);
    my_strcat(all_path, "/");
    my_strcat(all_path, entity->d_name);
    if (lstat(all_path, &all_info) == -1) {
        my_printf("Error getting information");
    } else {
        if (long_form) {
            print_info(all_path, &all_info);
        } else {
            my_printf("%s\n", all_path);
        }
    }
}

void full_process(const char *path, const char *file, char *all_path)
{
    my_strcpy(all_path, path);
    my_strcat(all_path, "/");
    my_strcat(all_path, file);
}

void process_dir(const char *path, int recurs, int hide, int long_form)
{
    DIR *dir = opendir(path);
    struct dirent *entity;
    char all_path[PATH];

    entity = readdir(dir);
    if (dir == NULL) {
        my_printf("Error by opening dir");
        return;
    }
    while (entity != NULL) {
        if (!hide && entity->d_name[0] == '.') {
            continue;
        }
        full_process(path, entity->d_name, all_path);
        file_process(path, entity, long_form);
        if (recurs && entity->d_type == 4)
            process_dir(all_path, recurs, hide, long_form);
    }
    closedir(dir);
}
