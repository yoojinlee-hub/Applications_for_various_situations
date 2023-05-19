#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void search_and_replace(char* dir_path, char* str_a, char* str_b);
void replace_filename(char* file_path, char* str_a, char* str_b);

int main() {
    // 문자열 A와 문자열 B를 입력 받음
    char str_a[100];
    char str_b[100];
    printf("문자열 A 입력: ");
    scanf("%s", str_a);
    printf("문자열 B 입력: ");
    scanf("%s", str_b);

    // 현재 디렉토리의 경로를 얻어옴
    char cur_dir[PATH_MAX];
    if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
        printf("Error: getcwd() failed\n");
        return 1;
    }

    // 디렉토리와 파일을 순회하며 문자열 A를 포함한 파일명에서 문자열 A만을 문자열 B로 변경함
    search_and_replace(cur_dir, str_a, str_b);

    printf("Done!\n");
    return 0;
}

void search_and_replace(char* dir_path, char* str_a, char* str_b) {
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

        // 파일인 경우 파일명을 변경함
        if (S_ISREG(file_stat.st_mode)) {
            replace_filename(path, str_a, str_b);
        }
        // 디렉토리인 경우 하위 디렉토리를 순회함
        else if (S_ISDIR(file_stat.st_mode)) {
            search_and_replace(path, str_a, str_b);
        }
    }

    closedir(dir);
}

void replace_filename(char* file_path, char* str_a, char* str_b) {
    char* file_name = strdup(file_path);
    char* new_name = NULL;

    if (file_name == NULL) {
        printf("Error: strdup() failed\n");
        return;
    }

    // 파일 이름에서 문자열 A를 찾아 문자열 B로 변경함
    char* name_start = strstr(file_name, str_a);
    if (name_start != NULL) {
        size_t len_a = strlen(str_a);
        size_t len_b = strlen(str_b);

        // 새로운 파일명을 생성함
        size_t new_name_len = strlen(file_name) - len_a + len_b + 1;
        new_name = malloc(new_name_len);
        if (new_name == NULL) {
            printf("Error: malloc() failed\n");
            free(file_name);
            return;
        }

        // 문자열 A를 찾은 위치 이전까지의 문자열을 복사함
        strncpy(new_name, file_name, name_start - file_name);
        new_name[name_start - file_name] = '\0';

        // 문자열 B를 새로운 파일명에 추가함
        strcat(new_name, str_b);

        // 문자열 A 다음부터의 문자열을 새로운 파일명에 추가함
        strcat(new_name, name_start + len_a);

        // 파일명을 변경함
        if (rename(file_path, new_name) != 0) {
            printf("Error: rename() failed\n");
        }
    }

    free(file_name);
    free(new_name);
}

