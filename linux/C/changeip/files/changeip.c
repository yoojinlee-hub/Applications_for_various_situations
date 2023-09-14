#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 파일 경로
    const char *file_path = "/lib/systemd/network/00-bmc-eth0.0.network";
    char new_address[256];

    // 사용자로부터 새로운 주소 입력 받기
    printf("Input New IPv4 address(ex: 192.168.0.100/24): ");
    fgets(new_address, sizeof(new_address), stdin);

    // 개행 문자 제거
    size_t len = strlen(new_address);
    if (len > 0 && new_address[len - 1] == '\n') {
        new_address[len - 1] = '\0';
    }

    // 파일 열기
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("[ERROR]Fail to open file\n");
        return 1;
    }

    // 임시 파일 생성
    FILE *temp_file = fopen("temp_file", "w");
    if (temp_file == NULL) {
        perror("[ERROR]Fail to make temp file\n");
        fclose(file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Address=") != NULL) {
            // "Address=" 라인을 찾으면 새로운 주소로 교체
            fprintf(temp_file, "Address=%s\n", new_address);
        } else {
            // 그 외의 라인은 그대로 복사
            fputs(line, temp_file);
        }
    }

    // 파일 닫기
    fclose(file);
    fclose(temp_file);

    // 원본 파일 삭제하고 임시 파일을 원본 파일로 이동
    if (remove(file_path) == 0 && rename("temp_file", file_path) == 0) {
        printf("Success to change address\n");
    } else {
        perror("[ERROR]Fail to change address\n");
    }

    return 0;
}

