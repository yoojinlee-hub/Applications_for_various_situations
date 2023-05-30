 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>

void search_files(char *dir_path, char *search_strings[], int num_strings);

int main() {
    int num_strings;
    printf("Enter the number of search strings: ");
    scanf("%d", &num_strings);
    getchar(); // Consume newline character

    char *search_strings[num_strings];
    for (int i = 0; i < num_strings; i++) {
        printf("Enter search string %d: ", i + 1);
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        search_strings[i] = strdup(buffer);
    }

    char cur_dir[PATH_MAX];
    if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
        printf("Error: getcwd() failed\n");
        return 1;
    }

    search_files(cur_dir, search_strings, num_strings);

    // Free allocated memory
    for (int i = 0; i < num_strings; i++) {
        free(search_strings[i]);
    }

    return 0;
}

void search_files(char *dir_path, char *search_strings[], int num_strings) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[PATH_MAX];

    if ((dir = opendir(dir_path)) == NULL) {
        printf("Error: opendir() failed\n");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &file_stat) == -1) {
            printf("Error: stat() failed\n");
            continue;
        }

        if (S_ISREG(file_stat.st_mode)) {
            FILE *fp = fopen(path, "r");
            if (fp == NULL) {
                printf("Error: fopen() failed\n");
                continue;
            }

            int found_all = 1;
            for (int i = 0; i < num_strings; i++) {
                if (strstr(entry->d_name, search_strings[i]) == NULL) {
                    found_all = 0;
                    break;
                }
            }

            if (found_all) {
                printf("%s/%s\n", dir_path, entry->d_name);
            }

            char line[BUFSIZ];
            while (fgets(line, sizeof(line), fp) != NULL) {
                for (int i = 0; i < num_strings; i++) {
                    if (strstr(line, search_strings[i]) != NULL) {
                        printf("%s/%s: %s", dir_path, entry->d_name, line);
                        break;
                    }
                }
            }

            fclose(fp);
        } else if (S_ISDIR(file_stat.st_mode)) {
            search_files(path, search_strings, num_strings);
        }
    }

    closedir(dir);
}

