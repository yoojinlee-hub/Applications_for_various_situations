#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

void search(char *dir_path, char *old_str, char *new_str);
void replace_in_file(char *file_path, char *old_str, char *new_str);
void *worker_thread(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [directory] [new string] [new string]\n", argv[0]);
        return 1;
    }

    char *dir_path = argv[1];
    char *old_str = argv[2];
    char *new_str = argv[3];

    search(dir_path, old_str, new_str);

    return 0;
}

void search(char *dir_path, char *old_str, char *new_str) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            // Skip "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            // Recursively search subdirectories
            char sub_path[PATH_MAX];
            snprintf(sub_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
            search(sub_path, old_str, new_str);
        } else if (entry->d_type == DT_REG) {
            // Create a worker thread to replace strings in file
            char file_path[PATH_MAX];
            snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, worker_thread, (void *) strdup(file_path));
            pthread_detach(thread_id);
        }
    }

    closedir(dir);
}

void *worker_thread(void *arg) {
    char *file_path = (char *) arg;
    char *old_str = "new string";
    char *new_str = "new string";

    replace_in_file(file_path, old_str, new_str);

    free(file_path);
    pthread_exit(NULL);
}
void replace_in_file(char *file_path, char *old_str, char *new_str) {
    FILE *fp = fopen(file_path, "r+");
    if (!fp) {
        perror("fopen");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        char *pos = strstr(line, old_str);
        if (pos) {
            // Calculate position of matched string
            int pos_offset = pos - line;

            // Overwrite matched string with new string
            fseek(fp, -(read - pos_offset), SEEK_CUR);
            fwrite(new_str, sizeof(char), strlen(new_str), fp);
            fseek(fp, read - pos_offset - strlen(old_str), SEEK_CUR);
        }
    }

    free(line);
    fclose(fp);
}
