#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void search_directory(const char* dir_path, const char* search_str);
void search_file(const char* file_path, const char* search_str);

int main(int argc, char* argv[]) {
    // 검색 대상 디렉토리와 검색할 문자열을 입력 받음
    if (argc != 3) {
        printf("Usage: %s <directory> <search_string>\n", argv[0]);
        return 1;
    }
    const char* dir_path = argv[1];
    const char* search_str = argv[2];

    // 디렉토리와 하위 디렉토리를 검색하며 문자열을 찾음
    search_directory(dir_path, search_str);

    printf("Done!\n");
    return 0;
}

void search_directory(const char* dir_path, const char* search_str) {
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

        // 파일인 경우 문자열을 검색함
        if (S_ISREG(file_stat.st_mode)) {
            search_file(path, search_str);
        }
        // 디렉토리인 경우 하위 디렉토리를 검색함
        else if (S_ISDIR(file_stat.st_mode)) {
            search_directory(path, search_str);
        }
    }

    closedir(dir);
}

void search_file(const char* file_path, const char* search_str) {
    FILE* fp;
    char line[BUFSIZ];
    int line_number = 1;
    int found = 0;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error: fopen() failed\n");
        return;
    }

    // 파일 내용을 한 줄씩 읽어서 검색 문자열을 찾음
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, search_str) != NULL) {
            printf("Found at: %s:%d\n", file_path, line_number);
            found = 1;
        }
        line_number++;
    }

    fclose(fp);

    if (!found) {
        printf("Not found in: %s\n", file_path);
    }
}

