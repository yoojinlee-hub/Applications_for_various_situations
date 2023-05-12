#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void search_replace(char* dir_path, char* str_a, char* str_b);
void replace_string(char* file_path, char* str_a, char* str_b);
void replace_filename(char* file_path, char* str_a, char* str_b);

int main(int argc, char* argv[]) {
    // 대상 문자열 A와 대체 문자열 B를 입력 받음
    if (argc != 5) {
        printf("Usage: %s -a A -b B\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-a") != 0 || strcmp(argv[3], "-b") != 0) {
        printf("Invalid argument\n");
        return 1;
    }
    char* str_a = argv[2];
    char* str_b = argv[4];

    // 현재 디렉토리의 경로를 얻어옴
    char cur_dir[PATH_MAX];
    if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
        printf("Error: getcwd() failed\n");
        return 1;
    }

    // 디렉토리와 파일을 순회하며 대상 문자열 A를 찾아 대체 문자열 B로 치환함
    search_replace(cur_dir, str_a, str_b);

    printf("Done!\n");
    return 0;
}

void search_replace(char* dir_path, char* str_a, char* str_b) {
    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    char path[PATH_MAX];

    if ((dir = opendir(dir_path)) == NULL) {
        printf("Error: opendir() failed\n");
        return;
    }

    // 디렉토리 안의 파일을 순회함
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &file_stat) == -1) {
            printf("Error: stat() failed\n");
            continue;
        }

        // 파일인 경우 문자열 치환과 파일 이름 변경을 수행함
        if (S_ISREG(file_stat.st_mode)) {
            replace_string(path, str_a, str_b);
            replace_filename(path, str_a, str_b);
        }
        // 디렉토리인 경우 하위 디렉토리를 순회함
        else if (S_ISDIR(file_stat.st_mode)) {
            search_replace(path, str_a, str_b);
        }
    }

    closedir(dir);
}
void replace_string(char* file_path, char* str_a, char* str_b) {
    FILE* fp;
    char buf[BUFSIZ];
    char* new_content = NULL;
    size_t new_size = 0;
    size_t content_size, len_a, len_b;
    int flag = 0;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error: fopen() failed\n");
        return;
    }

    // 파일 내용을 읽어 대상 문자열 A를 찾아 대체 문자열 B로 치환함
    while (fgets(buf, BUFSIZ, fp)) {
        content_size = strlen(buf);
        len_a = strlen(str_a);
        len_b = strlen(str_b);

        for (int i = 0; i <= content_size - len_a; i++) {
            if (strncmp(&buf[i], str_a, len_a) == 0) {
                new_content = realloc(new_content, new_size + i + len_b - flag);
                if (new_content == NULL) {
                    printf("Error: realloc() failed\n");
                    return;
                }
                memcpy(&new_content[new_size], buf + flag, i - flag);
                memcpy(&new_content[new_size + i - flag], str_b, len_b);
                new_size += i - flag + len_b;
                flag = i + len_a;
            }
        }

        if (flag != 0) {
            memcpy(&new_content[new_size], buf + flag, content_size - flag);
            new_size += content_size - flag;
            flag = 0;
        } else {
            new_content = realloc(new_content, new_size + content_size);
            if (new_content == NULL) {
                printf("Error: realloc() failed\n");
                return;
            }
            memcpy(&new_content[new_size], buf, content_size);
            new_size += content_size;
        }
    }

    fclose(fp);

    // 파일을 쓰기 모드로 열어 대체된 내용으로 덮어씌움
    fp = fopen(file_path, "w");
    if (fp == NULL) {
        printf("Error: fopen() failed\n");
        return;
    }
    fwrite(new_content, 1, new_size, fp);
    fclose(fp);

    free(new_content);
}
void replace_filename(char* file_path, char* str_a, char* str_b) {
    char* dir_path = strdup(file_path);
    char* file_name = strdup(file_path);
    char* new_name = NULL;

    if (dir_path == NULL || file_name == NULL) {
        printf("Error: strdup() failed\n");
        return;
    }

    // 파일 경로에서 디렉토리 경로와 파일 이름을 분리함
    char* dir_end = strrchr(dir_path, '/');
    if (dir_end != NULL) {
        *dir_end = '\0';
        file_name = dir_end + 1;
    }

    // 파일 이름에서 대상 문자열 A를 찾아 대체 문자열 B로 치환함
    char* name_end = strrchr(file_name, '.');
    if (name_end == NULL) {
        name_end = file_name + strlen(file_name);
    }
    char* name_start = file_name;
    size_t len_a = strlen(str_a);
    size_t len_b = strlen(str_b);

    while (name_start < name_end - len_a + 1) {
        if (strncmp(name_start, str_a, len_a) == 0) {
            new_name = realloc(new_name, name_start - file_name + len_b + strlen(name_end) + 1);
            if (new_name == NULL) {
                printf("Error: realloc() failed\n");
                return;
            }
            memcpy(new_name, file_name, name_start - file_name);
            memcpy(new_name + (name_start - file_name), str_b, len_b);
            memcpy(new_name + (name_start - file_name) + len_b, name_start + len_a, name_end - (name_start + len_a));
            memcpy(new_name + (name_end - file_name - len_a) + len_b, name_end, strlen(name_end) + 1);
            break;
        }
        name_start++;
    }

    // 대체 문자열 B가 발견되지 않았을 경우
    if (new_name == NULL) {
        free(dir_path);
        free(file_name);
        return;
    }

    // 파일을 새로운 이름으로 이동시킴
    char* new_path = malloc(strlen(dir_path) + 1 + strlen(new_name) + 1);
    if (new_path == NULL) {
        printf("Error: malloc() failed\n");
        free(new_name);
        free(dir_path);
        free(file_name);
        return;
    }
    sprintf(new_path, "%s/%s", dir_path, new_name);
    if (rename(file_path, new_path) == -1) {
        printf("Error: rename() failed\n");
    }
    free(new_path);
    free(new_name);
    free(dir_path);
    free(file_name);
}

